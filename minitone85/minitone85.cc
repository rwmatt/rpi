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
