#MCU=attiny85
AVRDUDEMCU=t85
CXX=avr-g++
CXX_FLAGS=-g -Os -Wall -mcall-prologues -mmcu=attiny85
#OBJ2HEX=/usr/bin/avr-objcopy
#AVRDUDE=/usr/local/bin/avrdude
TARGET=blinky
HEX=$(TARGET).hex

.PHONY: install clean

blinky.hex : blinky.o
	avr-objcopy -R .eeprom -O ihex $^ $@

blinky.o : blinky.cc
	$(CXX) $(CXX_FLAGS) -c blinky.cc 


install:$(HEX)
	install-hex $<

clean :
	rm -f *.hex *.obj *.o

# Maybe the rest is redundant??
#
noreset : all
	sudo avrdude -p $(AVRDUDEMCU) -P /dev/spidev0.0 -c linuxspi -b 10000 -U flash:w:$(TARGET).hex

fuse :
	sudo gpio -g mode 22 out
	sudo gpio -g write 22 0
	sudo avrdude -p $(AVRDUDEMCU) -P /dev/spidev0.0 -c linuxspi -b 10000 -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
	sudo gpio -g write 22 1


