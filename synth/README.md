# synth

## Introduction

**What is it?** A simplistic "synthesizer" that you can plug into your Arduino or Raspberry Pi. Use it when you want your project to produce a range of tones, and just want to use a tiny speaker instead of something more elaborate.

It should also work with many other microcontrollers or single-board computers with GPIO pins.

**What's the point?** My knowledge of electronics is bad, so I couldn't really figure out how to get good amplification on a speaker. Producing a consistent sound also proved to be difficult. Raspbian is not a RTOS (Real Time Operating System), which leads to glitchiness if you try to twiddle the pins directly. Execution of programs can be interrupted at any time, which messes with the timing. I toyed with using the [Ultibo](https://ultibo.org) unikernel, which worked better, but it still had the occasional problems with thread scheduling. One possible solution would be to dedicate a core to the task, but I could not get that working.

**Demo?**  Yes, on [YouTube](https://youtu.be/3DuH876Wv-k). It is also [discussed](https://www.raspberrypi.org/forums/viewtopic.php?f=38&t=230148) on a Raspberry Pi forum

**What you will need**
* An ATTiny85 chip, which is actually quite cheap and fun to play with. They are readily available, and only cost a quid. My favourite place to shop is RS Components. See [here](https://uk.rs-online.com/web/p/microcontrollers/1717852/), which gives you a pack of 5. What I love about these chips is that they do not take up much space on a circuit board, they are 5V tolerant, and can be run off a coin cell for really small projects
* a speaker. I chose a small passive speaker 32 ohm 0.5W  23mm (RS Component number 790-3975). Something similar should be perfectly OK. You can opt for a piezo-electric speaker, although I personally think they sound too harsh.
* An Arduino/Pi, according to taste, breadboard, resistors and accompanying wires.

## schematics

<img src="synth_bb_1.jpg" width="800">

```
           A85  (P85) BCM   (BRD) ARD  DUP
-----------------------------------------------
SPEAKER    PB1    (6)
SND_SS     PB2    (7)  16    (36)   8  Orange
SND_CLK    PB3    (2)  20    (38)   9  Yellow
SND_BYTE   PB4    (3)  21    (40)  10  Green

COLUMNS
A85 refers to pin NAMES on the ATTiny85
P85 refers to the physical pins on the ATTiny85
BCM are the GPIO pin numbers of a Pi3 (Raspberry Pi 3). E.g. "16" means GPIO16
BRD refers to the physical pin numbers on the Pi3
ARD refers to the physical pin numbers on an Arduino Uno R3
DUP refers to suggested lead colurings, which correspond to the the diagram above
```

Note that the speaker refers to a small passive speaker. The remaining lead needs to be
connected to GND on the controlling board. A piezo-electric speaker should also work, but
I dislike the quality of the sound produced.

You will also need to power and ground the ATTiny85, as indicated in the diagram.


## Programming the microcontroller

***Create the hex file***
You need to program the ATTiny85 chip with the synthesizer code in `slave.cc`. To compile it, type `make`. You will need the `avr-g++` and `avr-libc` libraries installed, which are readily available from the Raspbian repos.

***Hook up the ATTiny85 chip***
In order to install the hex file, you will need to put the chip on a breadboard and put on suitable resistors and wires. This is explained in an [instructable](http://www.instructables.com/id/Programming-the-ATtiny85-from-Raspberry-Pi/) 


***Install the hex file***
To install, type `make install`, which will copy the compiled hex file to the ATTiny85 chip.`make install` uses a file called `install-hex`, which is in "`/path/to/rpi/bin`", which you will need in your path.


## Demos and tests

You will first need to hook up the Arduino/Pi3 as shown in the schematics section.

***Arduino*** [sound-master.ino](sound-master/sound-master.ino) contains a simple sketch to produce a test pattern. Upload it in the usual way

***Pi3 demos***

Most of the demos should run "as-is" on Raspbian. If you want to run the piano demo, then you will need to install `pygame`:
```
sudo pip3 install pygame
```

* `master.py`: a simple test sequence similar to that for the Arduino. To run it: `python3 master.py`
* `enough.py`: a slightly more advanced test that plays "Just Can't Get Enough" by Depech Mode. The timing could be improved a little. It contains information about notes an frequencies. I have only entered a minimal number of notes. It might be a good idea to enhance this
* `piano.py`: turn your keyboard into a piano - well, sorta. I have only mapped a few basic notes, as described below. When I tried the program over VNC there were key repeats that I could not suppress. It should not be a problem for people using it from the PIXEL desktop environment, though.

```
KEY NOTE
g   G3
h   A3
j   B3
k   C4
```



