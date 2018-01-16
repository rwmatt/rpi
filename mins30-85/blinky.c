#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>

#include "millis.h"

typedef long int delay_t;
void delay_ms_a(delay_t ms)
{
	while(0<ms) {
		_delay_ms(10);
		ms -= 10;
	}
}

//void TinyTone(unsigned char divisor, unsigned char octave, unsigned long duration)
void TinyTone(unsigned char divisor, unsigned char octave, delay_t duration)
{
	TCCR1 = 0x90 | (8-octave); // for 1MHz clock
	// TCCR1 = 0x90 | (11-octave); // for 8MHz clock
	OCR1C = divisor-1;         // set the OCR
	delay_ms_a(duration);
	TCCR1 = 0x90;              // stop the counter
	    
}
	    

unsigned long startTime;
unsigned long mins30;
enum state {RUNNING, TIMES_UP} the_state;

void beepbeep(delay_t postDelay)
{
	//TinyTone(239, 4, 500);
	PORTB = 0xFF;
	_delay_ms(500);
	//TinyTone(239, 4, 500);
	PORTB = 0x00;
	_delay_ms(500);
	PORTB = 0xFF;
	_delay_ms(500);
	PORTB = 0x00;
	delay_ms_a(postDelay);
	//_delay_ms(postDelay);

}
void blink()
{
	//PORTB ^= 0xFF; // invert all the pins
	PORTB = 0xFF ; // set pins high
	_delay_ms(500); // wait some time
	PORTB = 0x00; // turn them off
	_delay_ms(500);
}

void loop()
{
	unsigned long timeNow = millis();
	switch(the_state) {
		case RUNNING:
			beepbeep(5000);
			if(timeNow >= startTime + mins30) the_state = TIMES_UP;
			break;
		case TIMES_UP:
			beepbeep(750);
			break;
	}

}

int main(void)
{
	DDRB = 0xFF; // PORTB is output, all pins
	PORTB = 0x00; // Make pins low to start

	//unsigned long startTime = timer0_millis;

	init_millis();
	startTime = millis();
	mins30 = (long) 30 * (long) 60 * (long) 1000; // 30 minutes

	for (;;) loop();
	return 0;
}
