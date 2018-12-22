CXX=avr-g++
CXX_FLAGS=-g -Os -Wall -mcall-prologues -mmcu=attiny85
BASE=minitone85

.PHONY: install clean

$(BASE).hex : $(BASE).o
	avr-objcopy -R .eeprom -O ihex $^ $@

main.o : main.cc
	$(CXX) $(CXX_FLAGS) -c main.cc 


install:$(BASE).hex
	install-hex $<

clean :
	rm -f *.hex *.obj *.o

