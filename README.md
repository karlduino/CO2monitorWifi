## CO<sub>2</sub> monitor based on an ESP32 and a SenseAir S8 sensor

This is a low-cost, do-it-yourself CO<sub>2</sub> monitor based on the [SenseAir
S8](https://senseair.com/products/size-counts/s8-lp/) sensor, an
[ESP32 microcontroller](), and a
16x2 LCD display.

Here I'm revising the [initial version of the project]() to work with an ESP32 (specifically
ESP-WROOM-32), which is a low-cost microcontroller that includes wifi
(and bluetooth). This will enable us to

The version of ESP32 I bought has 30 pins and holes at the corners,
and a micro-USB connector. It's a bit wider and longer than the
Arduino Nano we were using before, but it should fit in the same
space.

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
