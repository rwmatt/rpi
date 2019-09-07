# ATTiny85

Necessary, but not sufficient:
```sudo apt install avr-libc```

## pins

```
             _________
             |   U   |
Reset (PB5)  | 1   8 |  VCC (+)
(A3)   PB3   | 2   7 |  PB2 (A1, SCK, SCL)
(A2)   PB4   | 3   6 |  PB1 (PWM, MISO)
(-)    GND   | 4   5 |  PB0 (PWM, AREF, MOSI, SDA)
             _________

Notes:
* An means "Analog Input n"
* PB means PORTB
* PB5 requires a "fuse" to be set in order to use it.
```

## See also

* [clock](clock.md) - setting the clock frequency
* [carcoleptic](https://github.com/brabl2/narcoleptic) - 
reduced power consumption
* [megaphone](https://www.youtube.com/watch?v=__ECzxnvEg8)

