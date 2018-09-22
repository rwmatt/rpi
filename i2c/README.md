# I2C

## ATTiny85 slave


Install TinyWireS to the Arduino library folder:
```
git clone https://github.com/rambo/TinyWire.git
```



Connection guide
```
arduino | RPI      | ATTINY85
   3V3  | 1 3V3    | 8 VCC
A4 SDA  | 3 SDA.1  | 5 PB0
A5 SCL  | 5 SCL.1  | 7 PB2
   GND  | 6 GND    | 4 GND
```

## Arduino master

Install TinyWireM to the Arduino library folder:
```
git clone https://github.com/adafruit/TinyWireM.git
```

## Reference

* [ATtiny i2c Slave](https://thewanderingengineer.com/2014/02/17/attiny-i2c-slave/)

* [Getting to know the Raspberry Pi I2C bus](http://www.raspberry-pi-geek.com/Archive/2015/09/Getting-to-know-the-Raspberry-Pi-I2C-bus/(offset)/2)

* [Raspberry Pi I2C clock-stretching bug](http://www.advamation.com/knowhow/raspberrypi/rpi-i2c-bug.html)
