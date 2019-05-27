//#include <narcoleptic/Narcoleptic.h>
#include <Narcoleptic.h>

constexpr int bzr = PB1; // led was PB4

void setup() {
  // put your setup code here, to run once:

  // set clock to 1MHz
  cli();
  CLKPR = 1 << CLKPCE; 
  CLKPR = 3; 
  sei();

  pinMode(bzr, OUTPUT);

  // power-saving stuff
  Narcoleptic.disableMillis();
  Narcoleptic.disableTimer1();
  Narcoleptic.disableTimer2();
  Narcoleptic.disableSerial();
  //Narcoleptic.disableADC();  // !!! enabling this causes >100uA consumption !!!
  Narcoleptic.disableWire();
  Narcoleptic.disableSPI();
  // Another tweaks to lower the power consumption
  ADCSRA &= ~(1<<ADEN); //Disable ADC
  ACSR = (1<<ACD); //Disable the analog comparator
  // ATtiny25/45/85
  //Disable digital input buffers on all ADC0,ADC2,ADC3 + AIN1/0 pins
  DIDR0 = (1<<ADC0D)|(1<<ADC2D)|(1<<ADC3D)|(1<<AIN1D)|(1<<AIN0D);
}

void loop() {
  static int counter = 0;
  static int nbeeps = 1;
  for(int i = 0; i<nbeeps; ++i) {
    digitalWrite(bzr, HIGH);
    Narcoleptic.delay(100);
    digitalWrite(bzr, LOW);
    Narcoleptic.delay(100);    
  } 
  Narcoleptic.delay(5000);

  counter++;
  // beep twice after 30 minutes
  // 5200 is the total time for a loop to execute
  if(counter > float(60.0 * 30.0 /5.2)) {
    nbeeps =2;
  }
  
}
