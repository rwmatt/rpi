# Tiny RTC

* **clock** Arduino test
* **`urtc.py`** Adafruit library for interfacing with ESP8266. Copy it over to `/pyboard`

## Hookup

```
RTC:

+---------------------+
|                     |
|        ...          |
|       .   .         |
|       .   .         |
|        ...          |
|                     |
|  |   |   |   |   |  |
| GND VCC SDA SCL DS  |
+---------------------+

VCC s/b 5V
DS unused

     ARDUINO       ESP8266
SDA       A4     D2 (GPIO04)
SDL       A5     D1 (GPIO05)
```

![](rtc.jpg)

## See also


* [Tiny RTC](https://www.elecrow.com/wiki/index.php?title=Tiny_RTC) - module based on clock chip DS1307
