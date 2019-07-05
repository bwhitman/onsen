# onsen
Onsen Tamago Machine

## Parts

- some eggs and plain water
 - a small, cheap rice cooker: it should be under $20 US. It shouldn't have any screen or other features besides a switch to turn it on and off. [I use this one.](http://www.amazon.com/BLACK-DECKER-RC3303-1-5-Cup-Compact/dp/B002CVTT52/), which you can likely find at any drug store or housewares store. 
 - a PowerSwitch Tail from [Adafruit](https://www.adafruit.com/product/268), [SparkFun](https://www.sparkfun.com/products/10747) or [their own online store](http://www.powerswitchtail.com/Pages/default.aspx) There are two types of PowerSwitches: the standard one with a mechanical relay sold by Adafruit and Sparkfun, and then [one with a solid state relay](http://www.powerswitchtail.com/pages/powerssrtail.aspx) sold only on their website. The mechanical relay version makes a noticeable _click_ sound every time it cycles the power on and off -- dozens or even hundreds of times during an egg cook cycle -- but can safely handle a larger rice cooker with more wattage. The solid state one advises a maximum of 300 watts but is virtually silent. Check the wattage of your rice cooker. The rice cooker I recommend above is small -- can fit about six eggs and pulls exactly 300 watts, so I use the solid state PowerSwitch. I found the mechanical relay sound a bit too loud for the serene act of making an onsen egg. 
 - A microcontroller. To precisely follow these instructions, I suggest the [Arduino Pro Micro](https://www.sparkfun.com/products/12640) -- you can choose either the 5V or 3.3V version. But you can also use most any Arduino code-compatible variant if you already have one. I like the Pro Micro because it is small and cheap. The only real architecture dependent code on top of I2C and OneWire is a 15ms timer.
 - This [RTC breakout from Adafruit](https://www.adafruit.com/products/264) and the recomended coin cell battery. This is the thing that tells the eggs what time to heat up at. If the breakout is not in stock, it's a very simple circuit with parts you can easily acquire yourself. It looks like you can also subsitute the [DS3231 breakout](https://www.adafruit.com/products/3013), which seems smaller and higher accuracy, but I haven't tried it.
 - a DS18B20 temperature sensor on a waterproof cable from either [Adafruit](https://www.adafruit.com/products/381) or [Sparkfun](https://www.sparkfun.com/products/11050). You also need a single 4.7K resistor, which Adafruit (but not Sparkfun) ships with the sensor. 
 - This [LCD shield from Adafruit](https://www.adafruit.com/products/714). At $25, it's the most expensive single part, and it can be omitted if you're cutting costs. I really like seeing the status of the egg as it cooks, and the buttons are helpful to start or stop a cook in progress.
 - Some solder, a soldering iron, hookup wires, jumpers, etc. A breadboard or through hole drilled PCB would be helpful if you want to permanently attach the components.
 
 ## Put together

[See onsentamago.pro](http://onsentamago.pro/)
