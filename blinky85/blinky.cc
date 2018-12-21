#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

void delay_ms(unsigned long  ms)
{
	while(0<ms) {
		_delay_ms(10);
		ms -= 10;
	}
}

int main(void)
{
	DDRB = 0xFF;			// PORTB is output, all pins
	PORTB = 0x00;			// Make pins low to start

	while(true)
	{
		PORTB ^= 0xFF;		// invert all the pins
		delay_ms(5000); // delay 5s
	}
}
