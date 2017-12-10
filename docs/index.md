# Onsen Tamago Pro

Welcome to the wonderful world of automatic onsen eggs!


## Assmembly Instructions

It takes 6 steps to put together the Onsen Tamago Pro (OTP.) No tools are needed! Let's first make sure you have everything laid out:

 * **MCU**, the microcontroller, labeled "Pro Micro." It is the brains of the operation: it tells the display what to say, monitors the temperature and controls the rice cooker.
 * **CLOCK**, the thing with the little battery in it. It says RTC on it. It maintains the time, even when OTP is off. 
 * **DISPLAY**, the thing with the screen and buttons. 
 * **COOKER**, the rice cooker.
 * **TAIL**, the "PowerSwitch Tail," which handles all the dangerous voltages so you never have to. It turns the rice cooker on and off.
 * **TEMP**, the temperature sensor. That's the long cable with the metal end and the three wires at the other end. It goes in the water.
 * A protoboard, a white piece of plastic with lots of holes in it.
 * 10 different colored male-to-male jumper wires (pokey metal bits on each end) -- you won't need all of them!
 * A multi-colored (red, brown, yellow, orange) female-male jumper wire for the display (pokey metal on one end, receptacle on the other)
 * A tiny single 4.7Kohm **RESISTOR**, looks like a small brown bulb between two metal wires. This may be stuck in the protoboard for safe keeping, pull it out before beginning!
 * A Belkin power strip that has USB and AC power plug in. Keep this unplugged for now.
 * A black micro-USB cable.

### A note about safety

While this looks scary, the wires are all very low current and voltage. It should be no more dangerous than plugging an iPhone into a charger. The only bad part is safely encased in that big black "PowerSwitch Tail" box, which is screwed tightly shut and insulated. But of course, please watch carefully: it does heat up water to really hot temperatures automatically. We've been using ours every day for 2 years now with no incidents, and the eggs are delicious. This is a safe thing, but always follow some simple rules to make sure you're _double safe_:

 * Always keep the device unplugged while you're working on it. Don't have the USB cable or the Belkin thing plugged in until you're ready to test it. 
 * Unplug the power and USB cables if you're moving it. 
 * Don't touch any metal bits while the device is on. The buttons on the screen are safe to touch but don't move or touch any of the wires.
 * Carefully watch during your first egg to make sure it's all working -- you want the rice cooker to reach its target temperature and not go too far over. 
 * While using it, you want to make sure the temperature sensor is in the water at all times. If the temperature sensor falls out, the rice cooker will keep cooking until it boils the water, making for a bad egg! (It will shut itself off eventually if this happens, don't worry.) In normal use the temperature will be too hot to touch but not boiling.

### Step 1: Place 3 parts in the protoboard

The protoboard lets you make electrical circuits without any soldering. Protoboards are especially handy because each row of 5 pins (two columns, one on each side) are all connected to each other: without a wire, two adjacent pins can talk to each other. For example, pin # A1 talks to to B1, C1, D1, E1. And pin # F1 talks to to G1, H1, I1, J1. But F1 and A1 are not connected: they don't talk to each other without a wire. And A1 and A2 are not connected. In the picture below, those green lines mean the pins talk to each other without needing a wire!

![proto](pics/proto.png){:class="img-responsive" width="400px"}

I will give you explicit instructions on where to place things by naming the pin number, for example, A1 is the pin on the protoboard that is in column A and row 1 as labeled on the board. This makes it real easy to put everything together. But! After you successfully get it working, you will realize you can place the parts almost anywhere, to make it look nicer or use different wires, make it fit better in a case, etc. Feel free to change the positions of everything!

Each of the three parts (**MCU**, **DISPLAY** and **CLOCK**) has already been assembled and all you need to do is wire them up together! Let's start with **MCU**.

Put the **MCU** on the board so that the USB jack is facing down, towards row 30. You want the right column of pins on **MCU** all up and down column G, and the left column all up and down column C. Press until it's completely flat on the board, the pins should go all the way in. For example, the RAW pin on **MCU** should be seated in pin C30, and Pin 9 of **MCU** should be seated in pin G19. 

Put **CLOCK** on the board up and down column H, so that its GND pin is in pin H11, and SQW is in pin H15.

It should look like this:

![1](pics/1.jpg){:class="img-responsive" width="800px"}

### Step 2: Hook up the temperature sensor

**TEMP** has three wires coming out of it: red for voltage (5V, power), brown for ground (also written as GND, sort of "0" voltage), and lastly a wire for computer data (which tells the **MCU** how hot it is.) Let's put those wires in the right place. 

Wire **TEMP**'s brown cable (GND) to pin J28 (**MCU**'s GND.)

Wire **TEMP**'s yellow cable to J1.

Wire **TEMP**'s red cable to J4. 

We now need to wire up the **RESISTOR** so that the temperature sensor can talk the right voltage to **MCU**. If you remember, the protoboard talks across columns H and J without wires. You're adding a resistor between the computer data line and the voltage from **TEMP**.

Bend the resistor like a staple shape with two wire legs. Put one metal leg in H1, and the other metal leg in H4. We are adding the resistor between voltage and data of the sensor:   Press the resistor all the way down. It should look like this:

![2](pics/2.jpg){:class="img-responsive" width="800px"}

### Step 3: Hook up jumper wires

Now we need to connect the various parts to each other. We are going to run wires between parts that need to talk to each other. We'll use the pile of single wires for this step. Any color or length wire is fine, but save the multi-colored group of wires that are stuck together for the next step. 

Wire from G11 to I28. (**CLOCK**'s GND pin needs to be connected to **MCU**'s GND.)

Wire from G12 to B27. (**CLOCK**'s 5V pin needs to be connected to **MCU**'s VCC.)

Wire from G13 to J26. (**CLOCK**'s SDA pin needs to be connected to **MCU**'s SDA, which is on its pin 2)

Wire from G14 to J25. (**CLOCK**'s SCL pin needs to be connected to **MCU**'s SCL, which is on its pin 3.)

Wire from G1 to J24. (**TEMP**'s data pin needs to be connected to **MCU**'s pin 4.)

Wire from G4 to F12. (**TEMP**'s voltage pin needs to be connected to 5V, so we'll tap into **CLOCK**'s voltage.)

Looks like this:

![3](pics/3.jpg){:class="img-responsive" width="800px"}

### Step 4: Wire the display

Let's now use that 4 mutli-colored wire to make a cable between the display and the **MCU**.

Hold the display facing you, with the 5 buttons (arrow + select) facing on the bottom left. Count the pins on the bottom from the left and plug them into the mutli-colored wire:

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
 11. Don't connect
 12. Hook up the yellow wire (SDA)
 13. Hook up the orange wire (SCL)

Like this:

![4](pics/4.jpg){:class="img-responsive" width="800px"}

Now hook the other ends of those 4 wires to the right spots on the **MCU**:

Wire the red cable into A27. (5V to **MCU**'s VCC pin.)

Wire the brown cable into A29. (GND to **MCU**'s GND.)

Wire the yellow cable to I26. (SDA to **MCU**'s SDA / Pin 2.)

Wire the orange cable to I25. (SCL to **MCU**'s SCL / Pin 3.)

Like this:

![5](pics/5.jpg){:class="img-responsive" width="800px"}

### Step 5: Test the display, clock and temperature sensor.

You've wired most of it up, so let's take a break and test.

Plug the Belkin power strip into a wall or extension cable. Don't plug anything into its power input plug, but plug the USB cable from it into the **MCU**'s USB port. 

The screen should light up saying "Onsen Tamago Pro"! After a few seconds, it should show a clock, the current preset, and the start time. The clock should be correct and moving. 

If this **doesn't** happen, stop, check your work, and if it is still not working, give Brian a call. 

If it did happen, _great!_. Now let's test the temperature sensor. Press the "up" arrow button on the 4-way button pad. The screen should turn red, say "Heating", and the number on the right should be showing the current temperature the sensor is detecting, in celsius. If it is just laying on the table, it should say 22 or so. Hold it in your hand or breathe on it that number should slowly go up. If that works, _great!_. If it doesn't, or the temperature says 85 or 0 or something weird, stop, check your work and call Brian.

![6](pics/6.jpg){:class="img-responsive" width="800px"}

Unplug the USB cable from either end for your next and last step!

### Step 6: Wire up the rice cooker.

The **COOKER** and **TAIL** need to be connected now. The **TAIL** has a big power plug in, and big power plug out, and then two small wires coming out of its side, one black, and one white. The small wires are the way the **MCU** tells the rice cooker to turn on and off. 

Wire the small white wire to pin J23 / **MCU** pin 5.

Wire the small black wire to pin J27 / **MCU** GND.

With the Belkin power strip unplugged, plug the big power plug from **TAIL** into the Belkin input. And plug the **COOKER**'s power cable into the **TAIL** input power plug. 

Fill the **COOKER** with an egg and enough water to cover the egg.

Run the **TEMP** sensor through that hole in top of **COOKER**. There's a little rubber gasket to help it stay in there. You want the metal part of the **TEMP** sensor fully submersed in the water but the top cover on. 

Make sure the **COOKER**'s "WARM/COOK" switch is pushed down. 

Plug the USB cable into the Belkin power strip and the other end into the **MCU**.

Plug in the Belkin into wall power and the screen should turn on.

Now you're ready to make eggs!

![7](pics/7.jpg){:class="img-responsive" width="800px"}

I bet you could make a really cool box for the Onsen Tamago Pro, out of cardboard or wood or even a shoebox you have lying around! Just make sure to unplug the power and USB before moving it around. 

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







