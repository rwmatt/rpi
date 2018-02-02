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
PIN aka
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

