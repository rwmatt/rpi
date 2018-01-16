# GPIO

## Pin layout
```
3V3       1 ::  2   5V
GPIO2     3 ::  4   5V
GPIO3     5 ::  6   GND
GPIO4     7 ::  8   GPIO14
GND       9 :: 10   GPIO15
GPIO17   11 :: 12   GPIO28
GPIO27   13 :: 14   GND
GPIO22   15 :: 16   GPIO23
3V3      17 :: 18   GPIO24
GPIO10   19 :: 20   GND
GPIO9    21 :: 22   GPIO25
GPIO11   23 :: 24   GPIO8
GND      25 :: 26   GPIO7
ID_SD    27 :: 28   ID_SC
GPIO5    29 :: 30   GND
GPIO6    31 :: 32   GPIO12
GPIO13   33 :: 34   GND
GPIO19   35 :: 36   GPIO16
GPIO26   37 :: 38   GPIO20
GND      39 :: 40   GPIO21
```

## Example usage

Hook up an LED and resistor to GND (pin 6) and GPIO14 (pin 8).
Issue the following commands from Glute:
```
gfs 14 1
gos 14 0
```

`gfs` calls `SysGPIOFunctionSelect(pin, mode)`. Use `mode=0` for input,
and `mode=1` for output.

'gos` calls `SysGPIOOutputSet(pin, mode)`.
