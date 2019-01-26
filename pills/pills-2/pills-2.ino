#if defined(__AVR_ATtiny85__)
#define F_CPU 8000000UL
#endif

#include <avr/wdt.h>
#include <Narcoleptic.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

#include <util/delay.h>
void delay_ms(unsigned long  ms)
{
  Narcoleptic.enableMillis();
  while(0<ms--) _delay_ms(1);
  Narcoleptic.disableMillis();
}

void setupSleep() {
  //Serial.begin(9600);
  Narcoleptic.disableMillis(); //Do not disable millis - we need it for our delay() function.
  Narcoleptic.disableTimer1();
  Narcoleptic.disableTimer2();
  Narcoleptic.disableSerial();
  Narcoleptic.disableADC();  // !!! enabling this causes >100uA consumption !!!
  Narcoleptic.disableWire();
  Narcoleptic.disableSPI();

  // Another tweaks to lower the power consumption
  ADCSRA &= ~(1<<ADEN); //Disable ADC
  ACSR = (1<<ACD); //Disable the analog comparator
  //
#if !defined(__AVR_ATtiny85__)
  // ATmega48/88/168/328
  DIDR0 = B00111111; //Disable digital input buffers on all ADC0-ADC5 pins
  DIDR1 = (1<<AIN1D)|(1<<AIN0D); //Disable digital input buffer on AIN1/0
#else  
  //
  // ATtiny25/45/85
  //Disable digital input buffers on all ADC0-ADC3 + AIN1/0 pins
DIDR0 = (1<<ADC0D)|(1<<ADC2D)|(1<<ADC3D)|(1<<ADC1D)|(1<<AIN1D)|(1<<AIN0D);
#endif
  
}

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

void setup() {
  setupSleep();

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

void blinky() {
  //return;
  constexpr auto device = led;
  //constexpr auto device = buzzer;
   digitalWrite(device, HIGH);
   //Narcoleptic.enableMillis();
   delay_ms(5);
   digitalWrite(device, LOW);   
}

void loop() {
  checkAlarm();
  blinky();
  //Serial.println(i++);  
  //Serial.flush();
  Narcoleptic.sleep(WDTO_8S);

}
