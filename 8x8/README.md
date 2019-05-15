# 8x8

Adafruit LED 8x8 green matrix with I2C pack using HT16K33 controller

![](../8x8.jpg)

```
git clone https://github.com/adafruit/Adafruit_LED_Backpack
git clone https://github.com/adafruit/Adafruit-GFX-Library
```

Connection:
```
     8x8    Uno   ESP
+      +     5V   3V3 
-      -    GND   GND
SDA    D     A4    D2
SCL    C     A5    D1

ESP: nodemcu ESP8266
```

## HT16K33

It is the controller for the matrix. **Device ID 0x70**.

It transforms the input row in an odd way:
```
Input bytes:  76543210
Output bytes: 70123456
```


## nodemcu

Use `ledmat.py`. The 8x8 seems to be a bit picky with the pins it is compatible with, so stick
with those suggested above.

## See also

* [Datasheet](https://cdn-shop.adafruit.com/datasheets/ht16K33v110.pdf)
* [I2C with ATTiny85 on Adafruit Trinket](https://mythopoeic.org/adafruit-trinket-i2c/)

