#define F_CPU 8000000UL
#include "minitone85.h"

#include <avr/io.h>
#include <avr/iotnx5.h> // for CLKPR
#include <util/delay.h>

void delay_ms(unsigned long  ms)
{
	while(0<ms--) _delay_ms(1);
}



void chime(unsigned long freq, unsigned long duration = 250) 
{
	miniTone(freq); 
	delay_ms(duration);
	miniTone(0);
	delay_ms(25);
}

void airport_tanoy()
{
	chime(267);
	chime(328);
	chime(394);
	chime(532);
	delay_ms(1500);
	//delay_ms(50);
	/*
	   chime(2200, 500);
	   chime(0, 500);
	   chime(1100, 500);
	   chime(0, 1500);  
	   */
}

int main()
{

	// setup
	//CLKPR = 0x80;
	//CLKPR = 0;
	DDRB = 1<<PB1; // set PB1 for output
	PORTB = 0x00;  // set all pins low
	//pinMode(PB1, OUTPUT);

	//miniTone(1000); for(;;);

	// loop
	for(;;)  airport_tanoy();
}
