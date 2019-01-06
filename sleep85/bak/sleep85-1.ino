/*
   2019-01-05 mcarter created

   Ref:
   https://simple-ee.com/2017/05/16/arduino-attiny85-as-an-arduino-and-sleep-mode/
   https://www.re-innovation.co.uk/docs/sleep-modes-on-attiny85/
*/
#include <avr/sleep.h>
#include <avr/wdt.h>
//#include <avr/interrupt.h> 

// Routines to set and claer bits (used in the sleep code)
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

constexpr auto led = PB1;

// set system into the sleep state 
// system wakes up when wtchdog is timed out
void system_sleep() {
  
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF 
  //ADCSRA &= 0b01111111;
  //PRR=0xFF;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable(); 
  sleep_mode();                        // System actually sleeps here 
  sleep_disable();                     // System continues execution here when watchdog timed out   
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON  
}
 
// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {
 
  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;
 
  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}

void setup() {
  DDRB |= (1<<PB1); // set PB1 for output. Everything else is output
  //PORTB = 0xff; // set state high for everything (implies input pullup)
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Power down everything, will only wake up from WDT
  //sleep_enable(); // Enable sleep
  setup_watchdog(9);
}

void loop() {

  //DDRB = 0; // set everything as an input
  digitalWrite(led, 1);
  delay(5);
  digitalWrite(led, 0);

  system_sleep();
  

}
