## CO<sub>2</sub> monitor based on an ESP32 and a SenseAir S8 sensor

This is a low-cost, do-it-yourself CO<sub>2</sub> monitor based on the [SenseAir
S8](https://senseair.com/products/size-counts/s8-lp/) sensor, an
[ESP32 microcontroller](), and a
16x2 LCD display.

Here I'm revising the [initial version of the project](https://github.com/karlduino/CO2monitor) to work with an ESP32 (specifically
ESP-WROOM-32), which is a low-cost microcontroller that includes wifi
(and bluetooth). This will enable us to automatically log data
(including time stamps) to a google spreadsheet
using a google form.

The version of ESP32 I bought has 30 pins, holes at the corners for mounting,
and a micro-USB connector. It's a bit wider and longer than the
Arduino Nano we were using before, but it will fit in the same
plastic box we have been using.

### Parts

The cost of the main components is about US$ 60. (Most of the cost is
the sensor, which is about US$ 40. If you're willing to wait a month
for shipping, you can get it much cheaper
[via AliExpress](https://s.click.aliexpress.com/e/_9GsnY9)).

- [SenseAir S8](https://senseair.com/products/size-counts/s8-lp/)
  [![shopping cart icon](docs/pics/shopping-cart.png)](https://amzn.to/3AyzQMa)
  [![shopping cart icon](docs/pics/shopping-cart.png)](https://www.aliexpress.com/item/2255800698267376.html?spm=a2g0o.order_list.0.0.43ec1802aHp6qZ)

- ESP32 microcontroller
  (30-pin ESP-WROOM-32 version with mounting holes)
  [![shopping cart icon](docs/pics/shopping-cart.png)](https://amzn.to/3C4zNcW)
  [![shopping cart icon](docs/pics/shopping-cart.png)](https://www.aliexpress.com/item/3256803345493016.html?spm=a2g0s.8937460.0.0.470d2e0emtzhO6)

- 16x2 display with I2C chip
  [![shopping cart icon](docs/pics/shopping-cart.png)](https://amzn.to/3AwG3Z7)
  [![shopping cart icon](docs/pics/shopping-cart.png)](https://www.aliexpress.com/item/3256803324642290.html?spm=a2g0o.order_list.0.0.43ec1802aHp6qZ)

- clear plastic box
  [![shopping cart icon](docs/pics/shopping-cart.png)](https://amzn.to/3R24kxb)

To connect the components, you will need 9 female-to-female jumper
wires, solder, a soldering iron, 2 2-pin male headers for the SenseAir
S8 sensor, plus some 22-gauge wire to patch the LCD display and sensor
into a single pin, and 3/32" heat-shrink tubing to cover the solder join.

To attach components to the box, you'll need some small screws. I used
1/2" 4/40 screws for the LCD display and 5/8" 4/40 screws for the
microcontroller. Also some 1/4" plastic stand-offs. I get all this
from [McMaster-Carr](https://mcmaster.com).


### Code

You'll find the code on [Github](https://github.com/karlduino/CO2monitorWifi).

The first time using the ESP32 with the Arduino
IDE, you will need to install Board Manager and drivers for these boards.

My code for interacting with the SenseAir S8 sensor is based on the
example code in the [S8_UART
library](https://github.com/jcomas/S8_UART).

My code for writing on the LCD (including making custom characters)
was based on the example code with the older [LiquidCrystal_I2C
library](https://github.com/johnrickman/LiquidCrystal_I2C), though I
ended up using the [LCD_I2C
library](https://www.arduino.cc/reference/en/libraries/lcd_i2c/).



### Libraries

The code uses the following libraries to interact with the S8 sensor
and the LCD display, respectively.

- [S8_UART](https://www.arduino.cc/reference/en/libraries/s8_uart/)
  [![github](https://kbroman.org/icons16/github-icon.png)](https://github.com/jcomas/S8_UART)

- [LCD_I2C](https://www.arduino.cc/reference/en/libraries/lcd_i2c/)
  [![github](https://kbroman.org/icons16/github-icon.png)](https://github.com/blackhack/LCD_I2C)

- [EspSoftwareSerial](https://www.arduino.cc/reference/en/libraries/espsoftwareserial/)
  [![github](https://kbroman.org/icons16/github-icon.png)](https://github.com/plerup/espsoftwareserial)


### Assembly instructions

See the
[instructions document](https://karlduino.org/CO2monitorWifi/docs/instructions.html),
which attempts to explain the full process:

- Prepare the box (drilling holes to mount components and cutting
  holes for the sensor and the micro-USB cable)

- Solder headers onto the sensor and solder the patch-wire

- Mount the components in the box

- Make connections (see below)

- Load the software

- Adjust the contrast on the LCD display


### Connections

The 16x2 LCD with I2C has a 4-pin connector:

  - 16x2 GND → ESP32 GND (black)

  - 16x2 VCC → ESP32 VIN (red)

  - 16x2 SDA → ESP32 D21 (green)

  - 16x2 SCL → ESP32 D22 (yellow)

The SenseAir S8 needs soldering; you could use two pairs of header
pins. See page 3 of
[the
specs](https://rmtplusstoragesenseair.blob.core.windows.net/docs/publicerat/PSP126.pdf)
for pin information, which is not printed on the sensor.

- 4-pin side, pin 1 (G+) → ESP32 VIN (red)

- 4-pin side, pin 2 (G0) → ESP32 GND (black)

- 5-pin side, pin 2 (UART RX) → ESP32 D12 (green)

- 5-pin side, pin 3 (UART TX) → ESP32 D13 (yellow)


### License

This work released under the [MIT License](LICENSE.md).
