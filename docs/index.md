# Onsen Tamago Pro

Welcome to the wonderful world of automatic onsen eggs!


## Assmembly Instructions

### Safety lecture

While this looks scary, the wires are all very low current and voltage. It should be no more dangerous than plugging an iPhone into a charger. The only bad part is safely encased in that big black "PowerSwitch Tail" box, which is screwed tightly shut and insulated. But of course, please watch carefully and help out: it does heat up water to really hot temperatures automatically. We've been using ours every day for 2 years now with no incidents, and the eggs are delicious. 

This is a safe thing, but always follow some simple rules to make sure you're _double safe_:

 * Always keep the device unplugged while you're working on it. Don't have the USB cable or the Belkin thing plugged in until you're ready to test it. 
 * Unplug the power and USB cables if you're moving it. 
 * Don't touch any metal bits while the device is on. The buttons on the screen are safe to touch but don't move or touch any of the wires.
 * Carefully watch during your first egg to make sure it's all working -- you want the rice cooker to reach its target temperature and not go too far over. 
 * You want to make sure the temperature sensor is in the water at all times. 

### Place the PCBs in the protoboard. 

The white thing with all the holes is the "protoboard." It lets you make circuits without any soldering. Protoboards are especially handy because each row of 5 pins (two columns, one on each side) are all connected to each other. For example, pin # A1 is connected to B1, C1, D1, E1. And pin # F1 is connected to G1, H1, I1, J1. But F1 and A1 are not connected. And A1 and A2 are not connected. 

![proto](pics/proto.png){:class="img-responsive" width="800px"}


There are three PCBs for Onsen Tamago Pro (OTP.) They are:

 * **MCU**, the microcontroller, labeled "Pro Micro." It is the brains of the operation: it tells the display what to say, monitors the temperature and controls the rice cooker.
 * **CLOCK**, the thing with the little battery in it. It says RTC on it. It maintains the time, even when OTP is off. 
 * **DISPLAY**, the thing with the screen and buttons. 

There's also three other parts:

 * **COOKER**, the rice cooker.
 * **TAIL**, the "PowerSwitch Tail," which handles all the dangerous voltages so you never have to. It turns the rice cooker on and off.
 * **TEMP**, the temperature sensor. That's the long cable with the metal end and the three wires at the other end. It goes in the water.

Each PCB has already been assembled and all you need to do is wire them up together! Let's start with **MCU**.

Put the **MCU** on the board so that the USB jack is facing down, towards row 30. You want the right column of pins on **MCU** all up and down column G, and the left column along column C. Press until it's completely flush with the board. For example, the RAW pin on **MCU** should be seated in pin C30, and Pin 9 of **MCU** should be seated in pin G19. 

Put **CLOCK** on the board so that its GND pin is in pin H11, and SQW is in pin H15.

It should look like this:

![1](pics/1.jpg){:class="img-responsive" width="800px"}

### Hook up the temperature sensor

**TEMP** has three wires coming out of it: red for voltage (5V, power), brown for ground (also written as GND, and acts as the relative "0" for voltage), and lastly a wire for data (which tells the **MCU** how hot it is.) Let's put those wires in the right place. 

Wire **TEMP**'s brown cable (GND) to pin J28 (**MCU**'s GND.)

Wire **TEMP**'s yellow cable to J1.

Wire **TEMP**'s red cable to J4. 

There's a small resistor in the box. It looks like a tiny bulb between two metal wires. (It may be poked into the protoboard for safe keeping in transit.) We need to wire up the resistor so that the temperature sensor can talk the right voltage to **MCU**. Bend the resistor like a staple and put one metal wire in H1, and the other metal wire end in H4. We are adding the resistor between voltage and data of the sensor: if you remember, the protoboard connects across columns H and J.  Press the resistor all the way down. It should look like this:

![2](pics/2.jpg){:class="img-responsive" width="800px"}

### Hook up jumper wires

Now we need to connect the various PCBs to each other. We are going to run wires between parts that need to talk to each other. 

Any color or length wire is fine, but save the 4 wires that are stuck together for the next step. 

**CLOCK**'s GND pin needs to be connected to **MCU**'s GND. That's a wire from G11 to I28.

**CLOCK**'s 5V pin needs to be connected to **MCU**'s VCC. That's a wire from G12 to B27.

**CLOCK**'s SDA pin needs to be connected to **MCU**'s SDA pin 2. That's a wire from G13 to J26.

**CLOCK**'s SCL pin needs to be connected to **MCU**'s SCL pin 3. That's a wire from G14 to J25.

**TEMP**'s data pin needs to be connected to **MCU**'s pin 4. That's a wire from G1 to J24.

**TEMP**'s voltage pin needs to be connected to 5V, so we'll tap into **CLOCK**'s voltage. That's G4 to F12.

Looks like this:

![3](pics/3.jpg){:class="img-responsive" width="800px"}

### Wire the display

In the box are 4 wires bundled together that have a poky bit on one end and a receptacle on the other. Let's use those to make a cable between the display and the **MCU**.

Hold the display facing you, with the 5 buttons (arrow + select) facing on the bottom left. Count the pins on the bottom from the left and wire them like so.

 1. Don't connect
 2. Don't connect
 3. Hook up the red wire (5V)
 4. Don't connect
 5. Hook up the brown wire (GND)
 6. Don't connect
 7. (There is a space here, no pin)
 8. Don't connect
 9. Don't connect
 10. Don't connect
 11. Dont' connect
 12. Hook up the yellow wire (SDA)
 13. Hook up the orange wire (SCL)

Like this:

![4](pics/4.jpg){:class="img-responsive" width="800px"}

Now hook the other ends of those 4 wires to the right spots on the **MCU**:

Wire the red cable (5V) to **MCU**'s VCC pin. That's red to A27.

Wire the brown cable (GND) to **MCU**'s GND. That brown to A29.

Wire the yellow cable (SDA) to **MCU**'s SDA / Pin 2. That's yellow to I26.

Wire the orange cable (SCL) to **MCU**'s SCL / Pin 3. Thats orange to I25.

Like this:

![5](pics/5.jpg){:class="img-responsive" width="800px"}

### Test the display, clock and temperature sensor.

You've wired most of it up, so let's take a break and test.

Plug the Belkin power strip into a wall or extension cable. Don't plug anything into its power plug, but plug the USB cable from it into the **MCU**'s USB port. 

The screen should light up saying "Onsen Tamago Pro"! After a few seconds, it should show a clock, the current preset, and the start time. The clock should be correct and moving.

If this **doesn't** happen, stop, check your work, and give Brian a call. 

If it did happen, _great!_. Now let's test the temperature sensor. Press the "up" arrow button on the 4-way button pad. The screen should turn red, say "Heating", and the number on the right should be showing the current temperature the sensor is detecting, in celsius. If it is just laying on the table, it should say 22 or so. Hold it in your hand or breathe on it that number should slowly go up. If that works, _great!_. If it doesn't, or the temperature says 85 or 0 or something weird, stop, check your work and call Brian.

![6](pics/6.jpg){:class="img-responsive" width="800px"}

Unplug the USB cable from either end for your next and last step!

### Wire up the rice cooker.

The **COOKER** and **TAIL** need to be connected now. The **TAIL** has a big power plug in, and big power plug out, and then two small wires coming out of its side, one black, and one white. The small wires are the way the **MCU** tells the rice cooker to turn on and off. 

Wire the small white wire to pin J23 / **MCU** pin 5.

Wire the small black wire to pin J27 / **MCU** GND.

With the Belkin power strip unplugged, plug the big power plug from **TAIL** into the Belkin input. And plug the **COOKER**'s power cable into the **TAIL** input power plug. 

Fill the **COOKER** with an egg and enough water to cover the egg.

Run the **TEMP** sensor through that hole in top of **COOKER**. There's a little rubber gasket to help it stay in there. You want the metal part of the **TEMP** sensor fully submersed in the water but the top cover on. 

Make sure the **COOKER**'s "WARM/COOK" switch is pushed down. 

Make sure the USB cable is plugged into the Belkin power strip and the other end into the **MCU**.

Plug in the Belkin and the screen should turn on.

Now you're ready to make eggs!


![7](pics/7.jpg){:class="img-responsive" width="800px"}


## Usage

How to use the ONSEN TAMAGO PRO 🍳

The only thing you need to cook an EXCELLENT EGG is (1) some water in the pot, (2) between ONE and SIX eggs in the pot, and that (3) BLACK WIRE that’s going through the top completely immersed in the WATER. That’s the TEMPERATURE SENSOR. Also (4) make sure the COOKER’s switch is pushed down, towards COOK and away from KEEP WARM.

If you want an EXCELLENT EGG ready for you at 7 am, make sure the POWER CABLE is plugged into the wall. The screen should say what time it is now and also 6:00 AM (when it will start cooking.) You can choose your EGG TYPE: SAUCY, SUPPLE, SET or SEPHARDIC — by hitting the right and left ARROW KEYS under the screen. You can also choose OFF to not automatically cook at 6:00 AM.

 * SAUCY: a soft cooked egg where the yolk will be runny, yum
 * SUPPLE: a soft cooked egg where the yolk will be firmer but still soft, yum
 * SET: a medium cooked egg with a set yolk
 * SEPHARDIC: an egg that takes seven hours to cook! 

The machine first WARMS the water to your chosen setting, then COOKS the egg for 45 minutes (or 7 hours if SEPHARDIC), then KEEPS the egg for 3 hours at a lower temperature. You can eat the egg anytime after it COOKS. You'll notice the rice cooker's power light turning on and off a lot-- this is totally normal and how it works to keep the right temperature!

If you want an EXCELLENT EGG RIGHT NOW, make sure your EGG TYPE is chosen and then press the UP button to start IMMEDIATELY instead of at 6:00 AM.

If you want to CANCEL the cooking, hit the DOWN button at any time.

If the egg machine WORRIES you just pull the power cable out of the wall and it will not do anything.

ENJOY YOUR EGGS

## Expert mode

If you'd like to make changes to the software of OTP, you can simply download the [Arduino software](https://www.arduino.cc/en/Main/Software), wire the USB cable from **MCU** to your computer, and make changes to the [source code](https://github.com/bwhitman/onsen/blob/master/onsen.ino). Set the Arduino board type to Pro Micro 5V, 16MHz. 







