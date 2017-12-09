/*
 *  Onsen Tamago Pro
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

#define ONE_WIRE_BUS 4
#define RELAY_PIN 5

#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define MAX_PRESETS 4

#define TEMP_PRECISION 1.0

RTC_DS1307 RTC;
OneWire ds(ONE_WIRE_BUS);

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

double Setpoint = 0;
double Input;
double Output;

float heating_temp;
float keep_warm_temp = 54.444; // celsius
long cook_time_s = 45 * 60; // cooking time
long keep_warm_time_s = 180 * 60; // how long to keep warm
long heating_max_time_s = 25 * 60; // if heating is not done in this time, reset

volatile long onTime = 0;

// pid tuning parameters
double Kp = 2000;
double Ki = 0;
double Kd = 0;

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
int WindowSize = 10000;
unsigned long windowStartTime;

byte degree[8] = {B00110, B01001, B01001, B00110, B00000, B00000, B00000, B00000};
byte egg[8] =    {B00100, B01010, B10001, B10001, B10001, B10001, B10001, B01110};

enum state_list {WAITING = 0, HEATING, COOKING, KEEP_WARM, ERROR_MODE};
state_list state = WAITING;
long heating_start_ms;
long stable_start_ms;
long keep_warm_start_ms;


// what time to start cooking -- the date field (first 3 numbers) don't matter, just the time H:M
DateTime start_time = DateTime(2000, 1, 1, 6, 0, 0);

const char * presets[] = {
  "Off     ",
  "Saucy   ",
  "Supple  ",
  "Set     ",
  "Sephard ",
};

const float heating_temp_presets[] = {
  0,
  62.7778,
  65.5556,
  71.1111,
  72.5000,
};

const long cook_time_presets[] = {
  45 * 60,
  45 * 60,
  45 * 60,
  45 * 60,
  7 * 60 * 60,
};

byte selected_preset = 1;



SIGNAL(TIMER1_OVF_vect)
{
  DriveOutput();
  TCNT1 = 0xFB1E;
}


void logo() {
  lcd.setBacklight(WHITE);
  lcd.print("Onsen Tamago Pro");
  lcd.setCursor(4, 1);
  lcd.write(2); lcd.write(2);
  lcd.setCursor(10, 1);
  lcd.write(2); lcd.write(2);
}


void setup() {
  RTC.begin();
  // uncomment this to set the time from the compilation date of this file
  //delay(5000); // we sleep 5s because you flash it w/o connected to RTC, first run should be connected to the RTC
  //RTC.adjust(DateTime(__DATE__, __TIME__));


  heating_temp = heating_temp_presets[selected_preset];
  cook_time_s = cook_time_presets[selected_preset];
  pinMode(ONE_WIRE_BUS, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  lcd.begin(16, 2);
  lcd.createChar(1, degree);
  lcd.createChar(2, egg);

  logo();
  delay(5000);
  windowStartTime = millis();
  myPID.SetTunings(Kp, Ki, Kd);
  myPID.SetSampleTime(1000);
  myPID.SetOutputLimits(0, WindowSize);
  start_waiting();

  // Run timer1 interrupt every 15 ms
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 |= (1 << TOIE1);
  TCNT1 = 0xFB1E;
  TCCR1B |= (1 << CS12);


}

void start_error() {
  lcd.clear();
  state = ERROR_MODE;
  lcd.setBacklight(BLUE);
  digitalWrite(RELAY_PIN, LOW);
  Setpoint = 0;
  myPID.SetMode(MANUAL);
}

void start_heating() {
  lcd.clear();
  state = HEATING;
  lcd.setBacklight(RED);
  heating_start_ms = millis();
  Setpoint = heating_temp;
  myPID.SetMode(AUTOMATIC);
}

void start_cooking() {
  lcd.clear();
  state = COOKING;
  lcd.setBacklight(VIOLET);
  stable_start_ms = millis();
}

void start_keep_warm() {
  lcd.clear();
  state = KEEP_WARM;
  lcd.setBacklight(GREEN);
  Setpoint = keep_warm_temp;
  myPID.SetMode(AUTOMATIC);
  keep_warm_start_ms = millis();
}

void start_waiting() {
  lcd.clear();
  digitalWrite(RELAY_PIN, LOW);
  state = WAITING;
  lcd.setBacklight(WHITE);
  Setpoint = 0;
  myPID.SetMode(MANUAL);
}



void loop() {
  DoControl();
  char buffer[12];
  lcd.setCursor(0, 0);
  uint8_t buttons = lcd.readButtons();

  DateTime now = RTC.now();

  if (state == WAITING) {
    lcd.print("Preset: ");
    sprintf(buffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    lcd.print(buffer);
    if (selected_preset != 0) {
      if ((now.minute() == start_time.minute()) && (now.hour() == start_time.hour())) {
        start_heating();
      }
    }
  } else if (state == HEATING) {
    lcd.print("Heating up ");
    if ( (millis() - heating_start_ms) > (heating_max_time_s * 1000)) {
      // been heating too long, probably the temp sensor is not in water or the cooker is not heating properly
      start_error();
    } else {
      if (fabs (Input - heating_temp) < TEMP_PRECISION ) {
        start_cooking();
      }
    }
  } else if (state == COOKING) {
    lcd.print("Cooking ");
    lcd.write(2); lcd.print(" ");
    int time_left_s = cook_time_s - ((millis() - stable_start_ms) / 1000);
    sprintf(buffer, "%03d:%02d", time_left_s / 60, time_left_s % 60);
    lcd.print(buffer);
    if ( (millis() - stable_start_ms) > (cook_time_s * 1000)) {
      start_keep_warm();
    }
  } else if (state == KEEP_WARM) {
    lcd.print("Keeping ");
    lcd.write(2); lcd.print(" ");
    int time_left_s = keep_warm_time_s - ((millis() - keep_warm_start_ms) / 1000);
    sprintf(buffer, "%03d:%02d", time_left_s / 60, time_left_s % 60);
    lcd.print(buffer);
    if (millis() - keep_warm_start_ms > (keep_warm_time_s * 1000)) {
      start_waiting();
    }
  } else if (state == ERROR_MODE) {
    lcd.print("Error ");
  }

  lcd.setCursor(0, 1);
  if (state == WAITING) {
    lcd.print(presets[selected_preset]);
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

  if (state == WAITING) {

    if (buttons & BUTTON_UP) {
      if (selected_preset != 0) start_heating();
    }
    if (buttons & BUTTON_LEFT) {
      if (selected_preset == 0) {
        selected_preset = MAX_PRESETS;
      } else {
        selected_preset--;
      }
      heating_temp = heating_temp_presets[selected_preset];
      cook_time_s = cook_time_presets[selected_preset];
    }
    if (buttons & BUTTON_RIGHT) {
      if (selected_preset == MAX_PRESETS) {
        selected_preset = 0;
      } else {
        selected_preset++;
      }
      heating_temp = heating_temp_presets[selected_preset];
      cook_time_s = cook_time_presets[selected_preset];
    }

  } else {
    if (buttons & BUTTON_DOWN) { // stop, cancel
      start_waiting();
    }
  }

  //delay(10);
}


void DoControl() {
  if ((state != WAITING) && (state != ERROR_MODE)) {
    Input = getTemp();
    myPID.Compute();
    onTime = Output;
  }
}

void DriveOutput() {
  if ((state != WAITING) && (state != ERROR_MODE)) {
    long now = millis();
    if (now - windowStartTime > WindowSize) {
      windowStartTime += WindowSize;
    }
    if ((onTime > 100) && (onTime > (now - windowStartTime))) {
      digitalWrite(RELAY_PIN, HIGH);

    } else {
      digitalWrite(RELAY_PIN, LOW);

    }
  }
}



float getTemp() {
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad


  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;

}

