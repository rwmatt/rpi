# I2C

## Connection guide
```
ARDUINO | RPI      | ATTINY85
   3V3  | 1 3V3    | 8 VCC
A4 SDA  | 3 SDA.1  | 5 PB0
A5 SCL  | 5 SCL.1  | 7 PB2
   GND  | 6 GND    | 4 GND
```

## ATTiny85 slave


Install TinyWireS to the Arduino library folder:
```
git clone https://github.com/rambo/TinyWire.git
```

Compile the I2Cs.ino file using the Arduino IDE. It is a simple project. What it does is set a counter at 0. Each time the Tiny is woken up via I2C, it increments the counter, and sends back its value as a byte.

You can transfer the compiled hex file to the ATTiny85 using `install-hex` located in subfolder binky85. Ensure you have verbose compilation set on the IDE so that you know the name of the hex file.


## Arduino master

Install TinyWireM to the Arduino library folder:
```
git clone https://github.com/adafruit/TinyWireM.git
```

Connect the Arduino to the ATTiny85 using the connection guide (ignoring the RPI column).

Compile I2Cm.ino, and upload it to the Arduino. 

Open up the Arduino serial monitor. It should output the numbers 0..255 in a cycle to confirm that everything is working correctly.

## RPi master

Connect the RPi to the ATTiny85 using the connection guide above.

To connect to the board via python, type:
```
python3 i2cm.py
```

This basically does the same thing as the Arduino master.

Alternatively, you can do it via C:
```
gcc i2cm.c -o i2cm
./i2cm
```
You can see the devices attached to I2C (SDA.1 and SCL.1) by typing the command:
```
i2cdetect -y 1
```

Likewise for SDA.0/SCL.0.

## Reference

* [ATtiny i2c Slave](https://thewanderingengineer.com/2014/02/17/attiny-i2c-slave/)

* [Getting to know the Raspberry Pi I2C bus](http://www.raspberry-pi-geek.com/Archive/2015/09/Getting-to-know-the-Raspberry-Pi-I2C-bus/(offset)/2)

* [Raspberry Pi I2C clock-stretching bug](http://www.advamation.com/knowhow/raspberrypi/rpi-i2c-bug.html)
