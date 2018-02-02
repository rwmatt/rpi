# blinkt

Turning the blinkt on and off.

## Setup

Create menu item Accessories > Pimoroni Dashboard (seems optional):
```
sudo apt-get install pimoroni
```

Install the actual software:
```
sudo apt-get install python3-blinkt
```

## systemd setup

If you want light 0 to be green when the system is running, execute the following:

```
./install-service
sudo systemctl enable blinkt.service
sudo systemctl start  blinkt.service
```

## wiring

The Blinkt uses the SPI interface. Orient the device so that the bevelled edge is closest to the case of the RPi.

Pinout diagram:
```
PIN Notes
2   5V
6   GND
16  BCM23 (data)
18  BCM24 (clock)
```

[Reference](https://pinout.xyz/pinout/blinkt#)

NB: power the device using pin 2, not pin 4, which is also a 5V output on the RPi.

The code supplied on the aforementioned link appears to be wrong, and seems to contain a slight bug. However, its near equivalent, `example01.py`, does the same thing, and can be run by executing the following command:
```
sudo python3 example01.py
```

## Arduino


In Firefox, navigate to 
```
https://github.com/adafruit/Adafruit_DotStar/releases
```
and download the latest release in zip format.

Start the Arduino IDE.

Select menu item Sketch > Include library > Add .ZIP library ..., and select the zip archive you just downloaded. This will add the library under ~/Arduino/libraries/Adafruit_DotStar-1.03.

To test the device, open up the file: `~/Arduino/libraries/Adafruit_DotStar-1.0.3/examples/strandtest/strandtest.ino`. 

If necessary, comment out the line:
```
#include <Adafruit_CircuitPlayground.h>
```

Wire up the device as follows:
```
ARD BLI Notes
5V  2
GND 6
4   16  data
5   18  clock

Legend:
ARD: Arduino pin
BLI: Blinkt pin. See wiring section above for pin numbering
```

Upload the sketch onto the Arduino. The device should light up.


