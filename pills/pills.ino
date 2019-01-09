// still seem to be quirks:
// https://stackoverflow.com/questions/43249977/attiny-85-sleep-watchdog-time-changes-after-first-reset
// this may work better:
// https://folk.uio.no/jeanra/Microelectronics/ArduinoWatchdog.html
#if defined(__AVR_ATtiny85__)
#define F_CPU 8000000UL
#endif

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>


//#define USE_WATCHDOG

#if defined(__AVR_ATtiny85__)
#pragma message "Using watchdog register for ATTiny85"
//#include "/home/pi/repos/rpi/mins30-85/millis.h"
#include <util/delay.h>
void delay_ms(unsigned long  ms)
{
  while(0<ms--) _delay_ms(1);
}
#define WDTCSR WDTCR
#else
#define delay_ms delay
#endif

void configure_wdt(void)
{
 
  cli();                           // disable interrupts for changing the registers

  MCUSR = 0;                       // reset status register flags

                                   // Put timer in interrupt-only mode:                                       
  WDTCSR |= 0b00011000;            // Set WDCE (5th from left) and WDE (4th from left) to enter config mode,
                                   // using bitwise OR assignment (leaves other bits unchanged).
  WDTCSR =  0b01000000 | 0b100001; // set WDIE: interrupt enabled
                                   // clr WDE: reset disabled
                                   // and set delay interval (right side of bar) to 8 seconds

  sei();                           // re-enable interrupts 
}
/*
void initSleep()
{
  cli();
  // MCUSR = 0; // reset status 

 
  // Clear the reset flag. 
  MCUSR &= ~(1<<WDRF);
  
  // In order to change WDE or the prescaler, we need to
  // set WDCE (This will allow updates for 4 clock cycles).
  //
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  // set new watchdog timeout prescaler value 
  WDTCSR = 1<<WDP0 | 1<<WDP3; // 8.0 seconds 
  
  // Enable the WD interrupt (note no reset). 
  //WDTCSR |= _BV(WDIE);
}
*/

// how many times remain to sleep before wake up
int nbr_remaining; 

// Put the Arduino to deep sleep. Only an interrupt can wake it up.
void sleep(int ncycles)
{  
  nbr_remaining = ncycles; // defines how many cycles should sleep

  // Set sleep to full power down.  Only external interrupts or
  // the watchdog timer can wake the CPU!
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
 
  // Turn off the ADC while asleep.
  power_adc_disable();
 
  while (nbr_remaining > 0){ // while some cycles left, sleep!

  // Enable sleep and enter sleep mode.
  sleep_mode();

  // CPU is now asleep and program execution completely halts!
  // Once awake, execution will resume at this point if the
  // watchdog is configured for resume rather than restart
 
  // When awake, disable sleep mode
  sleep_disable();
 
  }
 
  // put everything on again
  power_all_enable();
 
}

/*
void enterSleep(void)
{
   // Enable the WD interrupt (note no reset). 
  //WDTCR |= (1<<WDIE); 
  
  //set_sleep_mode(SLEEP_MODE_PWR_SAVE);   // EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. 
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. 
  sleep_enable();
  
  // Now enter sleep mode. 
  sleep_mode();
  //sleep_cpu();
  
  // The program will continue from here after the WDT timeout
  sleep_disable(); // First thing to do is disable sleep. 
  
  // Re-enable the peripherals. 
  power_all_enable();
   // Disable the WDT interrupt 
  //WDTCR &= ~_BV(WDIE);
}
*/

constexpr auto led = PB3; // Arduino pin 3; tiny85 pin 2
constexpr auto buzzer = PB4; // Arduino pin 4; tiny85 pin 3

enum at_state {WAITING, ACTIVATED, DONE };

class At {
  public:
  int m_hour;
  at_state state; // = WAITING;
  void updateAt(int hr) {
    //Serial.println("update info:");
    //Serial.println(hr, DEC);
    //Serial.println(m_hour, DEC);
    
    if(hr < m_hour) { 
      //Serial.println("entering waiting state");
      state = WAITING;
    } else if(state == WAITING) {
      state = ACTIVATED;
    } else if(state == ACTIVATED) {
      //Serial.println("Called change state to done");
      state = DONE;
    }
    //if(state == ACTIVATED) state = DONE;
    //if(state == WAITING) state = ACTIVATED;
  }

};

static At ats[4];
volatile static bool already_setup = false;
void setup () {  

  if(already_setup) return;
  already_setup = true;
  #ifdef USE_WATCHDOG
  //initSleep();
  //wdt_enable(WDTO_2S);
  configure_wdt();
  #endif
  /*
  Serial.begin(9600);
  Serial.println("setup started");
  Serial.println("NB 1");
  */
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Wire.begin();
  RTC.begin(); 

  ats[0].m_hour = 9;
  ats[1].m_hour = 12;
  ats[2].m_hour = 17;
  ats[3].m_hour = 21;
}

void beep() {
  //Serial.println("beeping");
  //Serial.println(ACTIVATED);
  for(int i=0; i<4; i++) {
    digitalWrite(buzzer, HIGH);
    delay_ms(100);
    digitalWrite(buzzer, LOW);
    delay_ms(100);
  }
  //Serial.println("... finished beeping");
}

void checkAlarm() {
  DateTime now = RTC.now();
  bool alarm = false;
  for(auto& at:ats) {
    at.updateAt(now.hour());
    if(at.state == ACTIVATED) alarm = true;
    //Serial.print(at.state);
  }
  //Serial.println("");
  if(alarm) beep();  
}

void  prin_info() {
  #if defined(__AVR__ATtiny85__)
  return;
  #endif
    DateTime now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println(); 
    Serial.flush();
}    
void blinky() {
  //return;
  constexpr auto device = led;
  //constexpr auto device = buzzer;
   digitalWrite(device, HIGH);
   delay_ms(5);
   digitalWrite(device, LOW);   
}

void loop () {
  static unsigned char counter =0;
  //if(counter++ % 8 == 0) // every 64 seconds, which is about a minute
    checkAlarm();
    counter++;
    //digitalWrite(led, counter & 1);
  blinky();
  prin_info();
  #ifdef USE_WATCHDOG
  //enterSleep();
  sleep(1);
  #else
  delay_ms(1000);
  #endif
}

#ifdef USE_WATCHDOG
ISR(WDT_vect) {
  wdt_reset();
}
#endif
