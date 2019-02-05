#include <SPI.h>

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
void init_rtc(){  
    Wire.begin();
    RTC.begin();
    bool reset = ! RTC.isrunning();
    reset = 0;
    if (reset) {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
      }
}

void rtc_to_serial() {
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
}

void setup () {
  Serial.begin(9600);
  init_rtc();
  init_0seg();
}

// set dp to true to show decimal point
void show_dec(int pos, int val, bool dp = false) {
  int v = val%10;
  if(dp) v |= 1<<7; // add in the decimal point  
  maxTransfer(pos, v);
  //int v1 = val/10;
  maxTransfer(pos+1, val/10);
  
}
void write_to_0seg() {
  DateTime now = RTC.now();
  show_dec(1, now.minute());
  show_dec(3, now.hour(), true);
  maxTransfer(5, 0b1111); // blank
  maxTransfer(6, 0b1111); // blank
  show_dec(7, now.day());
}

void loop () {
  rtc_to_serial();
  write_to_0seg();
  Serial.flush();
  delay(1000);
}



// What pin on the Arduino connects to the LOAD/CS pin on the MAX7219/MAX7221
#define SS 10

void init_0seg() {
  pinMode(SS, OUTPUT);
  SPI.setBitOrder(MSBFIRST); // 0seg takes data in high bit first  
  SPI.begin();
  maxTransfer(0x0F, 0x00);
  maxTransfer(0x09, 0xFF); // Enable mode B  
  maxTransfer(0x0A, 0x0F); // set intensity (page 9)  
  maxTransfer(0x0B, 0x07); // use all pins    
  maxTransfer(0x0C, 0x01); // Turn on chip
}


  
  

/**
 * Transfers data to a MAX7219/MAX7221 register.
 * 
 * @param address The register to load data into
 * @param value   Value to store in the register
 */
void maxTransfer(uint8_t address, uint8_t value) {
  digitalWrite(SS, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(SS, HIGH); 
}
  
