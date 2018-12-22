CXX = avr-g++
CXXFLAGS = -g -Os -Wall -mcall-prologues -mmcu=attiny85
BASE=minitone

.PHONY: install clean

$(BASE).hex : $(BASE).elf
	avr-objcopy -R .eeprom -O ihex $^ $@

$(BASE).elf : main.o minitone85.o
	$(CXX) -g -mmcu=attiny85 $^ -o $@ 

.o :.cc
	$(CXX) -c $^ 


install:$(BASE).hex
	install-hex $<

clean :
	rm -f *.hex *.obj *.o *.elf

