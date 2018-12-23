#MCU=attiny85
#AVRDUDEMCU=t85
CXX = avr-g++
CXXFLAGS = -g -Os -Wall -mcall-prologues -mmcu=attiny85 -std=c++11
#OBJ2HEX=/usr/bin/avr-objcopy
#AVRDUDE=/usr/local/bin/avrdude
#TARGET=blinky

.PHONY : clean install

image.hex : image.elf
	avr-objcopy -R .eeprom -O ihex $^ $@

image.elf : main.o millis.cc ../minitone85/minitone85.o
	$(CXX) -g -mmcu=attiny85 $^ -o $@ 


.i : .cc
	$(CXX) -c $^

install : image.hex
	install-hex $<


clean :
	rm -f *.hex *.obj *.o $(TARGET)

