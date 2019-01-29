// Arduino Nano version only

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

constexpr auto buzzer = A2;

enum at_state {WAITING, ACTIVATED, DONE };

class At {
  public:
  int m_hour;
  at_state state = WAITING;
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
void setup () {  

  Serial.begin(9600);
  Serial.println("setup started");
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
    delay(1000);
    digitalWrite(buzzer, LOW);
    delay(1000);
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
/*
void blinky() {
  constexpr auto device = led;
  //constexpr auto device = buzzer;
  digitalWrite(device, HIGH);
  delay(5);
  digitalWrite(device, LOW);   
}
*/
void loop () {
  checkAlarm();
  //blinky();
  prin_info();
  delay(10000);
}
