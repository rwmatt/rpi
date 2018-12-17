/*
 * Started 20-Jan-2018
 * 
 * Meditation timer and white noise generator
 * 
 * You probably need the clock running at 8MHz
 * 
 * Incomplete
 */

#include <Arduino.h>
#if defined(__AVR_ATtiny85__)  
#define F_CPU 8000000UL
constexpr int buzzer = PB1;
constexpr int btn = PB4;
//static_assert(0, "X");
#else
constexpr int buzzer = 7;
constexpr int btn = 8;
#endif

typedef unsigned long ms_t;

//static_assert( defined(1), "NOT USING TINY");
//#include <util/delay.h>

ms_t startTime;

constexpr ms_t mins30 = 30UL * 60000UL; // 30 minutes
enum state {WHITE, START_TIMER, TIMING, TIMES_UP} the_state;

int numClicks = 0;
bool btnDown() { // return true if click down detected, false otherwise
  static ms_t start = 0;
  static bool prevState = HIGH, curState = HIGH;
  ms_t constexpr nap = 20; // to debounce
  ms_t now = millis();
  if(now - start < nap) return false;
  curState = digitalRead(btn);
  bool ret = false;
  if(curState == LOW and prevState == HIGH) { numClicks++; ret = true;}
  if(curState != prevState) start = now;
  prevState = curState;
  return ret;
}

bool pdelay(ms_t ms) { // true => completed, flase => interrupted
  //bool ret = false;
  ms_t start = millis();
  while(millis() - start < ms) {
    if(btnDown()) return false;
  }
  return true;
}

void miniTone(unsigned long freq)
{
  if(freq==0) {
    TCCR1 = 0x90; // stop the counter
    return;
  }
  
  freq *= 2;
  int prescale = 1;
  while(freq * 256 * (1<<(prescale-1)) < F_CPU)
    prescale++;
  TCCR1 = 0x90 | prescale;
  int divisor = F_CPU/(1<<(prescale-1))/freq;
  OCR1C = divisor;         // set the OCR
}

void setup()
{
#if defined(__AVR_ATtiny85__) 
  CLKPR = 0x80;
  CLKPR = 0;
#endif
  pinMode(btn, INPUT_PULLUP);
  startTime = millis();
  the_state = WHITE;
  pinMode(buzzer, OUTPUT);
  //digitalWrite(buzzer, HIGH);
  //Serial.begin(9600);
}

bool toner(ms_t freq, ms_t duration) {
  miniTone(freq);
  return pdelay(duration);
}
void beep(int postDelay)
{
  toner(750, 500) && toner(0, postDelay);
}
void beepbeep(int postDelay)
{
  beep(500);
  beep(postDelay); 
}

void loop()
{
  static int oldClicks = 0;
  btnDown();
  //Serial.println(numClicks);
  //Serial.println(digitalRead(btn));
  if(numClicks> oldClicks) {
    
    if(numClicks & 1) {
      the_state = START_TIMER;
      //Serial.println("set white");
    } else {
      the_state = WHITE;      
    }
    oldClicks = numClicks;
  }

  //return;
	ms_t timeNow = millis();
	switch(the_state) {
	  case WHITE:
	    //noTone(buzzer);
	    delayMicroseconds(100);
	    miniTone(random(400, 10000));
	    //digitalWrite(buzzer, random(2));
	    break;
	  case START_TIMER:
	    startTime = timeNow;
      the_state = TIMING;
      break;
		case TIMING:
			beepbeep(5000);
			if(timeNow >= startTime + mins30) the_state = TIMES_UP;
			break;
		case TIMES_UP:
		  the_state = WHITE;
			break;
	}

}
