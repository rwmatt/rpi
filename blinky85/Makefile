MCU=attiny85
AVRDUDEMCU=t85
CC=/usr/bin/avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=$(MCU)
OBJ2HEX=/usr/bin/avr-objcopy
#AVRDUDE=/usr/local/bin/avrdude
TARGET=blinky
HEX=$(TARGET).hex

.PHONY: install

all:
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)
	$(OBJ2HEX) -R .eeprom -O ihex $(TARGET) $(TARGET).hex
	rm -f $(TARGET)

install:$(HEX)
	install-hex $<

noreset : all
	sudo avrdude -p $(AVRDUDEMCU) -P /dev/spidev0.0 -c linuxspi -b 10000 -U flash:w:$(TARGET).hex

fuse :
	sudo gpio -g mode 22 out
	sudo gpio -g write 22 0
	sudo avrdude -p $(AVRDUDEMCU) -P /dev/spidev0.0 -c linuxspi -b 10000 -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
	sudo gpio -g write 22 1

clean :
	rm -f *.hex *.obj *.o

