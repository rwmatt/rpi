# Pills

A reminder to take your pills.

The code is a bit scrappy I'm afraid. I have been trying to get the watchdog sleep function to work, so far to no avail. The device currently uses about 2.4mA whilst idle, giving the battery life of 43 days, assuming the use of 2500mA rechargeable batteries. A properly-functioning watchdog should extend its expected life considerably.

## Components

* Tiny RTC I2C Module. It is a clock module. These have a coin cell attached so that it can keep track of the time
* LED
* 33R resistor
* PN2222A NON transistor
* Piezo buzzer
* ATTiny85 chip

## Schematics

![schematic](pills.svg)

## Implementation details

There is a class called `At`, which switches to `ACTIVATED` when its given hour is reached. You instantiate as many instances of this class as you require. In the code, I have set 4 alarms, to be activated at 8am, 12pm, 5pm, and 9pm.