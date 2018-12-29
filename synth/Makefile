CXX = avr-g++
CXXFLAGS = -g -Os -Wall -mcall-prologues -mmcu=attiny85 -std=c++14
BASE=minitone

.PHONY: install clean

image.hex : image.elf
		avr-objcopy -R .eeprom -O ihex $^ $@

image.elf : slave.o minitone85.o
		$(CXX) -g -mmcu=attiny85 $^ -o $@ 

.o :.cc
		$(CXX) -c $^ 


install:image.hex
		install-hex $<

clean :
		rm -f *.hex *.obj *.o *.elf

