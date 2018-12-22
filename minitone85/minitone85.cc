#include "minitone85.h"
#define F_CPU 8000000UL // ultra bad idea

//#define __AVR_ATtiny85__
#include <avr/io.h> // for TCCR1, OR1C

void miniTone(unsigned long freq)
{
	TCCR1 = 0; // stop the counter
	TCNT1 = 0;
	OCR1C = 0;
	if(freq==0) return;

	freq *= 2;
	int prescale = 1;
	while(freq * 256 * (1<<(prescale-1)) < F_CPU)
		prescale++;
	TCCR1 = 0x90 | prescale;
	int divisor = F_CPU/(1<<(prescale-1))/freq;
	OCR1C = divisor;         // set the OCR
}
