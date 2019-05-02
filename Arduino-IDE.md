# Arduino-IDE

Programming an Attiny85 from an Arduino, esp. wrt ATTiny85.

## Disable checks for updates

File : Preferences : Settings. Uncheck "Check for updates on startup"


## Setup
```
sudo usermod -a -G dialout <username>
sudo chmod a+rw /dev/ttyACM0
sudo chmod a+rw /dev/ttyUSB0
```

IDE 1.8.7 Default programmer is: AVRISP mkll

### Set up the ATTiny85 board:

File : Preferences : URL:
```
https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
```

Restart IDE


## Method 1

Source: [Programming an Attiny Arduino board (Using a DIY shield)](https://www.youtube.com/watch?v=7ntQDCIqioU&feature=youtu.be).

This is known to work on Arduino IDE version 1.8.5.

### 1. Add ATTiny85 Support to IDE

You only need to follow these steps once.

1. Select menu item: File > Preference, tab Settings
2. In "Addtional Board Manager URLs", add in:
```
https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
```
3. click OK.
4. Select menu item: Tools > Board > Board Manager
5. Scroll down, and select box labelled "attiny by David A. Mellis".
6. Click Install
7. close the dialog box.

### Create an ISP Sketch

ISP in this case means "In-system programming", not "Internet Service Provider".



5. Select menu item Files > Examples > ArduinoISP > ArduinoISP. 
This brings up a template sketch for an ISP.
6. Search for and uncomment the line so that it reads:
```
#define USE_OLD_STYLE_WIRING
```
7. You can either save that sketch under its old name (using menu item File > Save) , or save it as a new sketch (using menu item File > Save As) with a new name. For the purposes of argument, I will suppose that you save it as "ArduinoISP85".

Think of this sketch as a regular sketch.

### Upload the ISP sketch to the Arduino

You will need to perform this step as appropriate. You will need to perform it at least once in order to program your ATTiny85, and on any occasion in which you change the sketch on your Arduino. You do not necessarily need to upload it every time you want to reprogram you ATTiny85, though.

1. Ensure that there are no capacitors in the Arduino. A capacitor is used later to prevent a board reset. That is not relevant right now, though.
2. If necessary, open the "ArduinoISP85" (or similar) sketch created in the previous section.
3. Ensure the regular defaults are set, viz ...
4. Check that menu item Tools > Board is set to "Arduino/Genuine Uno"
5. Check that menu item Tools > Port is set "appropriately". On windows it will be `COMn` for some `n`. On Linux it will probably be `/dev/ttyACM0`.
6. Check that menu item Tools > Programmer is set to "AVR ISP".
7. Select menu item Sketch > Upload. This transfers your sketch onto the Arduino in the usual way, and makes it an ISP.


### Wire up a breadboard

```
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
```

... Instructions on creating physical shield ...

At 4:25: 



4:47 Add software for the ATTiny85 ...

5:00

Tools : Board : ATtiny25/45/85

Tools : Processor : ATtiny85

Tools : Clock : Internal 8MHz

Tools : Programmer : Arduino as ISP

Tools : Port : /dev/ttyACM0 (Arduino/Genuine Uno)

Tools : Burn Bootloader

## Arduino Commands

According to [this](http://www.hobbytronics.co.uk/arduino-attiny) page, the following commands are supported:


    pinMode()
    digitalWrite()
    digitalRead()
    analogRead()
    analogWrite()
    shiftOut()
    pulseIn()
    millis()
    micros()
    delay()
    delayMicroseconds()

The list was for Arduino version 0.022, so it is apt to be outdated.


## Schematic


<img src="ard.svg" width="800" />

The schematic was created using `fidocadj` using the source file `ard.fcd`.

