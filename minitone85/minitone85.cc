#define F_CPU 8000000UL

void setup() {
	CLKPR = 0x80;
	CLKPR = 0;
	pinMode(PB1, OUTPUT);    
}

void miniTone(unsigned long freq)
{
	TCCR1 = 0x90; // stop the counter
	if(freq==0) return;

	freq *= 2;
	int prescale = 1;
	while(freq * 256 * (1<<(prescale-1)) < F_CPU)
		prescale++;
	TCCR1 |= prescale;
	int divisor = F_CPU/(1<<(prescale-1))/freq;
	OCR1C = divisor;         // set the OCR
}

void chime(unsigned long freq, unsigned long duration) {
	miniTone(freq);
	delay(duration);
}
void loop()
{
	chime(2200, 500);
	chime(0, 500);
	chime(1100, 500);
	chime(0, 1500);  
}
