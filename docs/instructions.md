## CO2 monitor assembly instructions

The goal of this project was to have a low-cost but accurate CO2 monitor, so
that we could make a bunch of them to hand out to university workers
to assess the air quality in their work spaces, including classrooms.

### Overview

We got a pile of small clear plastic boxes, 3.2" square and a bit less
than 1" tall. It just barely fits the LCD display we're using.

We mount the LCD display and the ESP32 microcontroller inside the box, with the LCD
display facing down and out. We need to drill holes in the box for
screws to mount these components, plus a cut-out for the sensor (so
that it can get outside air) and a cut-out for a micro-USB
cable give power to the microcontroller.

There is a bit of soldering to do. We solder two pairs of header pins
onto the sensor, and then we solder some wires together so that both
the sensor and the LCD display can be plugged into the VIN pin on the
microcontroller. The drilling and cutting and soldering are the hard parts of
this assembly. We then connect the components with pre-made
female-to-female jumper cables.


### Prepare the box

There's a [drill
template](DrillTemplate/co2_monitor_drill_template.pdf) document
that you should print out, to guide the positioning of the drill holes
and cut-outs.

When printing the drilling template, make sure the sizes are kept the
same (in particular, don’t let it “shrink to printable area”).

Tape the template to the bottom of the box, making sure the lid
opening is oriented correctly. Most important is lining up the top and
sides, because the LCD display is a very tight fit. (The LCD display
we're using has screw holes separated by 75mm left/right and 31mm
top/bottom. If yours is different, you'll need to [adjust the R code
for the
template](https://github.com/karlduino/CO2monitorWifi/blob/main/docs/DrillTemplate/co2_monitor_drill_template.R)].

![box with drilling template](https://karlduino.org/CO2monitorWifi/docs/pics/01_with_template.jpg)

It seems easiest to use a 1/16" drill bit at each of the 9 spots (the
two holes for the LCD display, the three holes for the
microcontroller, and the four corners for the cut-out for the sensor).
We then go back and use a 7/16" drill bit to enlarge the holes for the
LCD display and the microcontroller. (The very small holes at the
corners of the cut-out are sufficient to get started cutting with a
small exacto blade.)

Before removing the template, mark the cut-out for the micro-USB with
a sharpie.

![box with marks for drilling and cutting locations](https://karlduino.org/CO2monitorWifi/docs/pics/02_with_marks.jpg)

When cutting in plastic, it can be useful to heat the blade with a
lighter or small torch.

For the cut-out for the sensor, I use a very thin exacto
saw (see the photo below). I drill 1/16” holes in the corners to
help get started with cutting the rectangle.

For the cut-out for the micro-USB, I don't use starting holes. I just
heat the blade and let it melt down through the plastic and then start
cutting.

I should have said: don’t forget to wear eye protection (for me,
reading glasses).

![box fully prepared, with my cutting tool and torch](https://karlduino.org/CO2monitorWifi/docs/pics/04_holes_cut.jpg)


### Solder headers onto the components

I’m using two pairs of breakaway male header pins for the S8 sensor.
It’s tricky to hold them in place for soldering. And you don't want to
mess up a US$ 40 sensor.

Page 3 of the [SenseAir S8
specs](https://rmtplusstoragesenseair.blob.core.windows.net/docs/publicerat/PSP126.pdf)
shows the pin assignments.

![SenseAir S8 pin assignments](https://karlduino.org/CO2monitorWifi/docs/pics/s8_pin_assignment.jpg)

We want two pins on the 4-pin side, which
nicely has labels: G+ on the end, on G0 next to it. Then we want two
pins on the 5-pin side, one in the middle (TX) and the one to its
right (RX), in the same direction as the ones we're doing on the 4-pin
side.

![SenseAir S8 laying so the side that gets air is face up, with a pairs of two-pin headers](https://karlduino.org/CO2monitorWifi/docs/pics/05_sensor_before_soldering.jpg)

The easiest way to hold the pins in place for soldering is probably
"blu tack": that blue putty for hanging posters.

![SenseAir S8 held in a vice with header pins held in place for soldering using blu tack](https://karlduino.org/CO2monitorWifi/docs/pics/06_blu_tack.jpg)

Here's the final product. It's not my best work, but the photo is
blurry enough that you can't tell.

![SenseAir S8 with a pair of two-pin headers soldered in place](https://karlduino.org/CO2monitorWifi/docs/pics/07_sensor_after_soldering.jpg)


We also need to solder three 22-gauge wires together, so that we can connect
both the LCD display and the sensor to the VIN pin on the
microcontroller. I cover the soldered joint with a bit of heat-shrink
tubing. (I used 3/32" tubing. You could maybe get by with 1/16" if your
soldering isn't as sloppy as mine.)

![3 22-gauge red wires soldered together, with the soldering joint covered with black heat-shrink tubing](https://karlduino.org/CO2monitorWifi/docs/pics/08_soldered_wires.jpg)



### Mount the components

To mount the LCD display, I use two 1/2” 4/40 screws with round heads,
with the heads outside the box, and two lock nuts.

The particular ESP32 microcontroller is a 30-pin "ESP-WROOM-32" with a
micro-USB connector and four big mounting holes.

![30-pin ESP-WROOM-32 microcontroller](https://karlduino.org/CO2monitorWifi/docs/pics/esp32.jpg)

To mount the ESP32 in the box, I use three 5/8” 4/40 screws with round
heads, and again with the heads outside the box, plus three 1/4"
plastic stand-offs, and three lock nuts. The stand-offs gets things
above where the pins are, and also should help prevent any
short-circuits from the metal screws.

I get these small screws and nuts
[McMaster-Carr](https://mcmaster.com).

![Plastic box with LCD display and ESP32 installed](https://karlduino.org/CO2monitorWifi/docs/pics/09_components_installed.jpg)


### Make connections

To make the connections, I use 9 female-to-female jumpers, 2 each of
three colors: green, yellow, and black, and then 3 red. They're not very
permanent and so they could eventually get rattled loose, but super
easy and allow us to reconfigure things if necessary.

You'll need to bend back the pins (carefully) on the LCD display, and
bend them apart a bit, two above and two below the blue contrast
potentiometer. Otherwise you won't be able to close the lid.

The 16x2 LCD with I2C has a 4-pin connector:

  - 16x2 GND → ESP32 GND (black)

  - 16x2 VCC → red patch wire → ESP32 VIN (red)

  - 16x2 SDA → ESP32 D21 (green)

  - 16x2 SCL → ESP32 D22 (yellow)

![Connections between LCD display and ESP32](https://karlduino.org/CO2monitorWifi/docs/pics/10_lcd_connections.jpg)

The SenseAir S8 connections are the following:

- 4-pin side, pin 1 (G+) → red patch wire → ESP32 VIN (red)

- 4-pin side, pin 2 (G0) → ESP32 GND (black)

- 5-pin side, pin 2 (UART RX) → ESP32 D12 (green)

- 5-pin side, pin 3 (UART TX) → ESP32 D13 (yellow)

![Connections between ESP32 and both the LCD display and sensor](https://karlduino.org/CO2monitorWifi/docs/pics/11_all_connections.jpg)

### Load the software

Download and install the [ESP32
IDE](https://www.arduino.cc/en/software).

Open the IDE, and then use a micro-USB cable to connect your ESP32
to your computer.

Then select your board: On the menu bar, click Tools, and then

**TO DO** need to explain how to download the board manager
(looking for ESP32-WROOM-D2)

**TO DO** also explain the libraries you need to get

Under Port (farther down in the Tools menu), you need to select the
USB port for your board. Hopefully you'll see just one choice. If you
have trouble, try using a different micro-USB cable.

Download the software for this project,
[`CO2monitorWifi.ino`](https://raw.githubusercontent.com/karlduino/CO2monitorWifi/main/CO2monitorWifi.ino)
and open it in the Arduino IDE. Upload it to your board, and it'll
immediately start running.


### Adjust the LCD contrast

You will need to adjust the contrast on the LCD display, using a small
screw to turn the blue potentiometer on the back of the LCD, near the
wire connections. You probably won't be able to read the text on the
display until you do. It may work to just turn it all the way to the
right (clockwise).



### Tape the box shut

If it's working, you can arrange the sensor as you want it and then
delicately close the lid so the wires all stay inside and so they push
the sensor down against its opening.

Tape the box shut with a couple of pieces of clear tape.


### Power the project

You use the same micro-USB port to power the project as to load the
software. You should be able to use any USB power adapter. You should
also be able to use any USB backup battery, like for a phone. Or you
can power the project with a USB port in a computer.



### Calibration

These SenseAir S8 including some auto-calibration feature (automatic
baseline calibration, ABC). It
considers past measurements and makes an
adjustment up or down, changing the baseline by no more than about
30-50 ppm every 8 days.

If you place the CO2 monitor outside in fresh air for 5-10 minutes, it
should give a reading that is close to 420 ppm. If it reads much too
high or much too low (say 320 or 500), you may want to manually
calibrate it.

I did this by soldering three 22-gauge solid wires together and then
using that plus a female-to-female jumper.

Turn the unit off. Remove the black wire from the SenseAir S8 sensor
and plug the three-way wire there and into ground on the ESP32. Turn
the unit back on; it should be working normally. And you have one open
connection on your three-way wire. You then when to touch that to the
5th pin on the 5-pin side of the SenseAir S8 (two holes away from your
yellow wire, on the other side from the green wire) for more than 4 seconds
but less than 8 seconds, so go for 5 seconds. It should then reset the
baseline to 400 ppm. (If you connect the pin to ground for more than 8
seconds, it will reset the baseline to 0 ppm, which you definitely
don't want.)

![Wires for calibration: three solid wires soldered together plus a female-to-female jumper](https://karlduino.org/CO2monitorWifi/docs/pics/12_wires_for_calibration.jpg)

So, following the [instructions from
Mariete](https://emariete.com/en/meter-co2-esp8266-nodemcu-sensor-senseair-s8/#Calibracion_del_sensor_de_CO2_Senseair_S8_LP):

- Leave the monitor running, out in fresh air, for 15 minutes

- Touch that bCal_in pin to ground for 5 seconds

- Let it continue running for another 5 minutes

That's the theory, anyway.
