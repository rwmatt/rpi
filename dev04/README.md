# dev04

Programming an ATTiny85 from the Pi.

Include files are in <code>/usr/lib/avr/include</code>, which I ascertained by typing <code>dpkg-query -L avr-libs</code>

## The board


<img src="dev04.svg" width="800">

Hooking up the Pi to the board:

```
PI          | DUPONT | BOARD
------------|--------|------
15  GPIO15  | Brown  | PCB1
17  3V3     | Red    | PCB2
19  MOSI    | Orange | PCB5
21  MISO    | Yellow | PCB4
23  SCLK    | Green  | PCB3
25  GND     | Black  | PCB6
```

The connections are unintuitive because I messed up the soldering. Grr.

Orient the board (aka device) so that the pins are on the left, and the chip slot is on the right. The '85 should be inserted with pin1 (A1) on the top left.

Prefixes R are for Raspberry Pi pins, B for the board pins of the dev device, and A for the tiny85 pins. 

Thus R25 is the Pin 25 (GND) on the Pi. 

Bn is the numbering-scheme for the board, with B1 at the top, and B6 at the bottom.

Connect Dupont cables to the Pi using the suggested colour scheme. Hook up the board in a similar manner.

## The bridge

<img src="bridge.jpg">

### Testing

Use project blinky85 for test purposes.

## References

* [instructable](http://www.instructables.com/id/Programming-the-ATtiny85-from-Raspberry-Pi/) 
