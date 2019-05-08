#include <SPI.h>

#include "maltime.h"

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
void init_rtc(){  
    Wire.begin();
    RTC.begin();
}

typedef unsigned long ulong;

//constexpr int sw2 = 16 ; // Nano A2
constexpr int btnBlue = 3 ; // Nano D3
constexpr int buzz = 8 ; // Nano D8

static bool timing = false; // are we timing, or displaying normal time
unsigned long timer_start;
bool timer_expired = false;

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
    Serial.flush();
}

void setup () {
  Serial.begin(9600);
  init_rtc();
  init_0seg();
  pinMode(btnBlue, INPUT_PULLUP);
  pinMode(buzz, OUTPUT);
  digitalWrite(buzz, LOW);
  write_to_0seg();
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
  int d = now.day();
  int mo = now.month();
  int hr = now.hour();
  adjBST(now.year(), mo, d, hr);
  show_dec(1, now.minute());
  show_dec(3, hr, true);
  maxTransfer(5, 0b1111); // blank
  maxTransfer(6, 0b1111); // blank
  show_dec(7, d);
}


// blank out the entire display
void cls0() {
  for(char i = 1; i<= 8; ++i) maxTransfer(i, 0b1111);    
}

class Every {
  public:
  Every(ulong interval) : interval(interval) {}
  bool rising();
  private:
  ulong interval, start = millis();
  
};
bool Every::rising() { 
    ulong now = millis();
    if(now<start) { start = now; } // timer overflow
    if(now-start < interval) return false;
    start = now;    
    return true; 
}


int toint(char* buf, int offset) {
  return 10*(buf[offset]-'0') + (buf[offset+1] -'0');
}
void  process_serial(){
  char buf[20], ch;
  if(!Serial.available()) return;
  int i= 0;
loop: 
  if(!Serial.available()) goto loop; 
  ch = Serial.read();
  //Serial.print(ch);
  if(ch == '\n') ch = 0;
  buf[i] = ch;
  if(ch == 0) goto eol;
  i++;
  i = min(i, sizeof(buf) -2);
  goto loop;
eol:

  if(buf[0] == '?') rtc_to_serial();
  if(buf[0] == 'H') { Serial.println("HELLO"); }
  if(buf[0] == '!') {
    // set the time
    int yr  = toint(buf, 3) + 2000;
    int mth = toint(buf, 5);
    int dy  = toint(buf, 7);
    int hr  = toint(buf, 9);
    int mn  = toint(buf, 11);
    int sec = toint(buf, 13);
    DateTime dt{yr, mth, dy, hr, mn, sec};
    RTC.adjust(dt);
    Serial.println("OK"); 
  }
  //Serial.read(); // newline expected
  Serial.flush();
  buf[0] = 0;
  
}


void nudge1 () { // tweak the time: 1 second every 3 hours
  static bool nudged = false;
  DateTime dt = RTC.now();
  int s = dt.second();
  if(s <30) return;
  if(dt.hour() != 0) { nudged = false; return; }
  if(nudged) return;
  Serial.println("nudge1()");
  Serial.flush();
  nudged = true;
  int offset = -13 + random(0,1); // calculated on 02-May-2019
  DateTime dt1{dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), s + offset};
  RTC.adjust(dt1);  
}

void change_major_mode()
{
  timing = ! timing;
  if(timing) {
    timer_start = millis();
    cls0();
    print_elapsed_time();
  } else {
    write_to_0seg();
  }
}
void tmrButton()
{
  static uint8_t btnState = 0x00;
  static bool released = true;
  btnState = (btnState << 1) + (LOW == digitalRead(btnBlue) ? 1 : 0);
  //Serial.println(btnState);
  if(btnState == 0xFF && released) {
    released = false;
    change_major_mode();
    //Serial.println("button activated");
    //Serial.println(digitalRead(btnBlue));
  }
  if(btnState == 0x00) released = true;    
}

void print_elapsed_time()
{
  ulong elapsed_time = (millis() - timer_start)/1000;
  timer_expired = elapsed_time > 1800; // 30 mins
  elapsed_time %= 3600; // lop off beyond hour
  show_dec(1, elapsed_time % 60); // show seconds
  show_dec(3, elapsed_time / 60, true); // show mins with dec point    
}

void beep(){  
  digitalWrite(buzz, HIGH);
  delay(100);
  digitalWrite(buzz, LOW);
  delay(100);
}

void ev_buzz() {
  if(!timing) return;
  beep();
  if(timer_expired) beep();
}

void loop () {
  //Serial.println(digitalRead(sw2));
  process_serial();
  static Every tmr{3};
  if(tmr.rising()) tmrButton();
  static Every evbuzz{5000};
  if(evbuzz.rising()) ev_buzz();

  static Every prtimer{500};
  if(prtimer.rising() && timing) print_elapsed_time();
  
  static Every ev{5000};
  if(!ev.rising()) return;  
  nudge1();
  if(!timing) write_to_0seg();
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
  maxTransfer(0x0A, 0x08); // set intensity (page 9)  
  maxTransfer(0x0B, 0x07); // use all pins    
  maxTransfer(0x0C, 0x01); // Turn on chip
  cls0();
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
  
