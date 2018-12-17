#define F_CPU 8000000UL

void setup() {
  CLKPR = 0x80;
  CLKPR = 0;
  pinMode(PB1, OUTPUT);  
  miniTone(5000); // 5kHz
  // TCCR1 = 0x90 // stop the counter

}

void miniTone(unsigned long freq)
{
  freq *= 2;
  int prescale = 1;
  while(freq * 256 * (1<<(prescale-1)) < F_CPU)
    prescale++;
  TCCR1 = 0x90 | prescale;
  int divisor = F_CPU/(1<<(prescale-1))/freq;
  OCR1C = divisor;         // set the OCR
}

void loop()
{
}
