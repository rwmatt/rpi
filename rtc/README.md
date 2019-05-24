# Tiny RTC

* **clock** Arduino test
* **`urtc.py`** Adafruit library for interfacing with ESP8266

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
