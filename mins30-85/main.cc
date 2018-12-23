#define F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>
#include "../minitone85/minitone85.h"

#include "millis.h"

void delay_ms(unsigned long  ms)
{
	while(0<ms--) _delay_ms(1);
}


unsigned long startTime;
constexpr unsigned long mins30  = (long) 30 * (long) 60 * (long) 1000; // 30 minutes
enum state {RUNNING, TIMES_UP} the_state;


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
}

void loop()
{
	unsigned long timeNow = millis();
	switch(the_state) {
		case RUNNING:
			miniTone(1000);
			delay_ms(500);
			miniTone(0);
			delay_ms(3000);
			if(timeNow >= startTime + mins30) the_state = TIMES_UP;
			break;
		case TIMES_UP:
			airport_tanoy();
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

	for (;;) loop();
	return 0;
}
