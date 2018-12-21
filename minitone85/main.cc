#define F_CPU 8000000UL
#include "minitone85.h"

#include <avr/io.h>
#include <util/delay.h>

void chime(unsigned long freq, unsigned long duration) 
{
	miniTone(freq);
	_delay_ms(duration);
}

int main()
{

	// setup
	CLKPR = 0x80;
	CLKPR = 0;
	DDRB = 1<<PB1; // set PB1 for output
	PORTB = 0x00;  // set all pins low
	//pinMode(PB1, OUTPUT);

	// loop
	for(;;) {
		chime(2200, 500);
		chime(0, 500);
		chime(1100, 500);
		chime(0, 1500);  
	}
}
