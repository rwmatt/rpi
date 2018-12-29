/*  Started mcarter 2018-12-28
 *  
 *  Get the latest version from
 *  https://github.com/JChristensen/tinySPI/releases
 */

//#include <tinySPI.h>
#include <stdint.h>
#include <avr/io.h>
#include "minitone85.h"

constexpr auto SND_SS = PB2;
constexpr auto SND_CLK = PB3;
constexpr auto SND_BYTE = PB4;



bool high(uint8_t pin) { return PINB & (1<<pin) ; }
bool low(uint8_t pin)  { return high(pin) != 0; }

bool poll_sound(uint16_t& value ) {
	//static uint16_t test = 0;
	if(high(SND_SS)) return false;
	//test += 400;
	//miniTone(test);
	value = 0;
	for(uint8_t i = 0; i< 16; i++) {
		while(!high(SND_CLK)); // wait for clock to be high
		value <<= 1; //shift value;
		if(high(SND_BYTE)) value +=1;
		while(high(SND_CLK)); // wait for clock to be low    
	}  
	while(!high(SND_SS));	
	//test += 400;
	//miniTone(test);
	return true;
}

void loop()
{
	uint16_t value;
	if(!poll_sound(value)) return;
	miniTone(value);

}
int main()
{
	//SPI.begin();
	DDRB = 0; // all pins default to input
	DDRB |= 1<<PB1; // speaker must be on PB1
	PORTB |= 1<<SND_SS; // pull up the selector to high

	// wait for master to be ready
	//while(low(SND_SS));

	for(;;) loop();

}

