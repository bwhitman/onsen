/* 
 *  Onsen Tamago Machine
 *  Brian Whitman
 *  brian@variogr.am
 *  December 2015
 */
 
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_RGBLCDShield.h>
#include <PID_v1.h>
#include <RTClib.h>

#define ONE_WIRE_BUS 2
#define ONE_WIRE_PWR 3
#define ONE_WIRE_GND 4
#define RELAY_PIN 7

#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define TEMP_PRECISION 1.0

RTC_DS1307 RTC;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempSensor;
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

double Setpoint = 0; 
double Input;
double Output;

float heating_temp = 62.7778;
float keep_warm_temp = 54.444;
long cook_time_s = 45 * 60;
long keep_warm_time_s = 99 * 60;

volatile long onTime = 0;

// pid tuning parameters
double Kp = 2000;
double Ki = 0;
double Kd = 0; 

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
int WindowSize = 10000; 
unsigned long windowStartTime;


byte degree[8] = {B00110,B01001,B01001,B00110,B00000,B00000,B00000,B00000};
byte egg[8] =    {B00100,B01010,B10001,B10001,B10001,B10001,B10001,B01110};

enum state_list {WAITING=0, HEATING, COOKING, KEEP_WARM};
state_list state = WAITING;

long stable_start_ms;
long keep_warm_start_ms;

DateTime start_time = DateTime(2000,1,1,17,0,0);

SIGNAL(TIMER2_OVF_vect) {
    DriveOutput();
}

void logo() {
  lcd.setBacklight(WHITE);
  lcd.print("Onsen Tamago Pro");
  lcd.setCursor(4,1);
  lcd.write(2); lcd.write(2);
  lcd.setCursor(10,1);
  lcd.write(2); lcd.write(2);
}

void setup() {
  // uncomment this to set the time from the compilation date of this file 
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  
  Serial.begin(9600);
  
  pinMode(ONE_WIRE_BUS, INPUT);
  pinMode(ONE_WIRE_GND, OUTPUT);
  digitalWrite(ONE_WIRE_GND, LOW);
  pinMode(ONE_WIRE_PWR, OUTPUT);
  digitalWrite(ONE_WIRE_PWR, HIGH);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  lcd.begin(16, 2);
  lcd.createChar(1, degree);
  lcd.createChar(2, egg);
  RTC.begin();
  logo();
  
  sensors.begin();
  if (!sensors.getAddress(tempSensor, 0)) lcd.print("Sensor error");
  sensors.setResolution(tempSensor, 12);
  sensors.setWaitForConversion(false);
  delay(5000);
  //lcd.setBacklight(OFF);
  lcd.clear();
  windowStartTime = millis();   
  myPID.SetTunings(Kp,Ki,Kd);
  myPID.SetSampleTime(1000);
  myPID.SetOutputLimits(0, WindowSize);
  start_waiting();
  
  
  // Run timer2 interrupt every 15 ms 
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1<<TOIE2;
}

void start_heating() {
  state = HEATING;
  lcd.setBacklight(RED);
  Setpoint = heating_temp;
  myPID.SetMode(AUTOMATIC);
}

void start_cooking() {
  state = COOKING;
  lcd.setBacklight(VIOLET);
  stable_start_ms = millis();
}

void start_keep_warm() {
  state = KEEP_WARM;
  lcd.setBacklight(GREEN);
  Setpoint = keep_warm_temp;
  myPID.SetMode(AUTOMATIC);
  keep_warm_start_ms = millis();
}
void start_waiting() {
  digitalWrite(RELAY_PIN,LOW);
  state = WAITING;
  lcd.setBacklight(WHITE);
  Setpoint = 0;
  myPID.SetMode(MANUAL);
}


char buffer[9];

void loop() {
  DoControl();
  lcd.setCursor(0,0);
  
  uint8_t buttons = lcd.readButtons();
  
  DateTime now = RTC.now();
  
  if(state == WAITING) {
    lcd.print("      ");
    sprintf(buffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second()); 
    lcd.print(buffer);
    if ((now.minute() == start_time.minute()) && (now.hour() == start_time.hour())) {
      lcd.clear();
      start_heating();
    }
  } else if(state == HEATING) {
    lcd.print("Heating up ");
    if (fabs (Input - heating_temp) < TEMP_PRECISION ) {
      lcd.clear();
      start_cooking();
    }
  } else if(state == COOKING) {
    lcd.print("Cooking ");
    lcd.write(2); lcd.write(2); lcd.print(" ");
    int time_left_s = cook_time_s - ((millis() - stable_start_ms) / 1000);
    sprintf(buffer, "%02d:%02d", time_left_s / 60, time_left_s % 60);
    lcd.print(buffer);
    if( (millis() - stable_start_ms) > (cook_time_s * 1000)) {
      lcd.clear();
      start_keep_warm(); 
    }
  } else if(state == KEEP_WARM) {
    lcd.print("Keeping ");
    lcd.write(2); lcd.write(2); lcd.print(" ");
    int time_left_s = keep_warm_time_s - ((millis() - keep_warm_start_ms) / 1000);
    sprintf(buffer, "%02d:%02d", time_left_s / 60, time_left_s % 60);
    lcd.print(buffer);
    if(millis() - keep_warm_start_ms > (keep_warm_time_s * 1000)) {
      lcd.clear();
      start_waiting();
    }
  }

  lcd.setCursor(0,1);
  if(state == WAITING) {  
    lcd.print("Start ");
    sprintf(buffer, "%02d:%02d:%02d", start_time.hour(), start_time.minute(), start_time.second()); 
    lcd.print(buffer);
  } else {
    lcd.print(Setpoint);
    lcd.write(1);
    lcd.print(F(" | "));  
    lcd.print(Input);
    lcd.write(1);
    lcd.print(" ");
  } 
    
 
  if(buttons & BUTTON_LEFT) { // status
    Serial.println("Status");
    Serial.print("Input ");
    Serial.println(Input);
    Serial.print("Setpoint ");
    Serial.println(Setpoint);
    Serial.print("Output ");
    Serial.println(Output);
  }
}


void DoControl() {
  if (sensors.isConversionAvailable(0)) {
    Input = sensors.getTempC(tempSensor);
    sensors.requestTemperatures();
  }
  myPID.Compute();
  onTime = Output; 
}

void DriveOutput() {  
  if(state != WAITING) {
    long now = millis();
    if(now - windowStartTime>WindowSize) { 
       windowStartTime += WindowSize;
    }
    if((onTime > 100) && (onTime > (now - windowStartTime))) {
       digitalWrite(RELAY_PIN,HIGH);
    } else {
       digitalWrite(RELAY_PIN,LOW);
    }
  }
}

