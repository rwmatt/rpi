# Arduino-85

Programming an Attiny85 from an Arduino

## Method 1

Source: [Programming an Attiny Arduino board (Using a DIY shield)](https://www.youtube.com/watch?v=7ntQDCIqioU&feature=youtu.be).


Wire up a breadboard:
85 -> Ard
1 -> 10 
4 -> GND
5 -> 11
6 -> 12
7 -> 13
8-> 5V


On Arduino: 10uF:
+ -> reset
- -> gnd (white stripe)

Optionally:
85:1 -> LED + -> resistor (e.g. 220 ohm) 86:5

... Instructions on creating physical shield ...

At 4:25: 

Plug in Arduino UNO

Start Arduino IDE 1.8.5

File : Examples : ArduinoISP : ArduinoISP . This brings up a sketch into the IDE.

4:36 Line 81 Uncomment the line so that it reads
#define USE_OLD_STYLE_WIRING

File: Save As : ArduinoISP85

4:47 Add software for the ATTiny85 ...

5:00

Tools : Board : ATtiny25/45/85

Tools : Processor : ATtiny85

Tools : Clock : Internal 8MHz

Tools : Programmer : Arduino as ISP

Tools : Port : /dev/ttyACM0 (Arduino/Genuine Uno)

Tools : Burn Bootloader




