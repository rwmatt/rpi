# DEV02 home-made board

A board that can be used for testing purposes

## Schematics

![schematic](XC4870.png)

The schematic diagram (`XC4870.Srmsgl`) was made using `xcircuit`.


## mhz85

This is an experiment to see if the bootloader can be reprogrammed, and to check the clock speed. It can!

The sketch for this is [`mhz85.ino`](mhz85.ino).

Operating it:
* In the Arduino IDE, set the clock speed that you want to use.
* upload the sketch onto an ATTiny85
* transfer the chip onto the board, and power up. The red LED should be on, and the green one off.
* Click the button and start a stopwatch simultaneously. The red LED goes off, and the green one goes on. The chip does a lengthy computation to test the clock speed.
* Wait until the red LED comes on again. Stop the stopwatch and record the time.

Results:

| Clock  | Time |
|--------|-----:|
| 1 MHz  | 146s |
| 8 MHz  |  18s |

* "Clock" is the clock speed set when uploading the sketch
* "Time" is the time taken to run the calculation (lower is faster).

Note that 146/18 = 8.1, which confirms that the 8MHz version is running 8X faster than the 1MHz version.

## green_red

Performs a slow alernation between green and red LED. Speeds up whilst the button is held down.

The sketch for this is [`green_red.ino`](green_red.ino).

