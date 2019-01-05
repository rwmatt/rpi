# rpi
My Arduino, ATTiny85 and RPi (Raspberry Pi) projects

Reference information:


* [GPIO-pi3](GPIO-pi3.md): pin diagram for RPi3.
* [i2c](i2c/README.md): Arduino and RPi3 master, ATTiny85 slave

## Arduino

* [IDE](Arduino-IDE.md) : general setup, particularly about ATTiny85 setup.
* [Multithreading](https://create.arduino.cc/projecthub/reanimationxp/how-to-multithread-an-arduino-protothreading-tutorial-dd2c37)

## ATTiny85

Necessary, but not sufficient:
```sudo apt install avr-libc```

Projects:
* blinky85: directory containing code for blinking an ATTiny85 from RPi - consider it a baseline for making projects using C++, and not the IDE
* [clock85](clock85.md): setting the clock
* [dev01](dev01/README.md): projects for the DEV01 home-made board: om
* [dev02](dev02/README.md): projects for the DEV02 home-made board: green_red and mhz85
* ~~[dev04](dev04/README.md): Programming an ATTiny85 from the Pi~~
* [dev05](dev05/README.md): ATTiny85 programmer for the Pi
* [minitone85](minitone/README.md): working version of tone() for ATtiny85
* [mins30-85](mins30-85/README.md): 30 minute timer for ATTiny85, programmable from the Pi (`DEV01`). 
* [synth](synth/README.md): sound synthesizer/tone-maker
* [sleep85](sleep75/README.md): conserving power by putting the ATTiny85 to sleep

Technical:
* [pins85](pins85.md):pinout diagram for ATTiny85


## General

* [debounce](https://gist.github.com/blippy/430cc73cb50b9e44c3423c2717ab22ee) a button

## Projects

### blinkt

Playing around with [blinkt](blinkt/README.md).

### edikit1

Projects that come to mind with the CamJam EduKit 1, including
a self-destruct sequence [here](edukit1/README.md).




## Hardware

* [Explorer HAT pro](https://shop.pimoroni.com/products/explorer-hat)
* [Microcontroller - Introduction - A Beginners Guide to the Atmel AVR Atmega32](https://newbiehack.com/MicrocontrollerIntroductionABeginnersGuidetotheAtmelAVRAtmega32.aspx)
* [Mini USB nano](https://www.ebay.co.uk/sch/i.html?_from=R40&_trksid=m570.l1313&_nkw=mini+usb+nano&_sacat=0). Amazon also has good deals on these
