#pragma once
/*********************************

millis.h - sample Arduino millis()
on an ATTiny85 (use timer0 overflow 
interrupt do to it) 

Code in public domain
**********************************/

//#define F_CPU 1000000UL
//#include <avr/io.h>
//#include <inttypes.h>
//#include <avr/interrupt.h>

/* prototypes */
//void setup(void);
//void loop(void);
//int main(void);

/* some vars */
//uint64_t _millis = 0;
//uint16_t _1000us = 0;

//uint64_t old_millis = 0;

/* interrupts routines */
// timer overflow occur every 0.256 ms
/*
ISR(TIM0_OVF_vect) {
	_1000us += 256;
	while (_1000us > 1000) {
		_millis++;
		_1000us -= 1000;
	}
}
*/

// safe access to millis counter
uint64_t millis();
void init_millis();
