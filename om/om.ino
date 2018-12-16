/*
 * Started 20-Jan-2018
 * 
 * Meditation timer
 * 
 * You probably need the clock running at 8MHz
 * 
 * Incomplete
 */
 
#define F_CPU 8'000'000UL
//#include <util/delay.h>

unsigned long startTime;
constexpr int buzzer = 7;
constexpr unsigned long mins30 = 30L * 60L * 1000L; // 30 minutes
enum state {RUNNING, TIMES_UP} the_state;

void setup()
{
  startTime = millis();
  the_state = RUNNING;
  pinMode(buzzer, OUTPUT);
  //digitalWrite(buzzer, HIGH);
}

void beep(int postDelay)
{
  /*
  analogWrite(buzzer, 210); // about the right volume. 150 is too low. 
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(postDelay);
  */
  tone(buzzer, 750);
  delay(500);
  noTone(buzzer);
  delay(postDelay);
}
void beepbeep(int postDelay)
{
  beep(500);
  beep(postDelay); 
}

void loop()
{
	unsigned long timeNow = millis();
	switch(the_state) {
		case RUNNING:
			beepbeep(5000);
			if(timeNow >= startTime + mins30) the_state = TIMES_UP;
			break;
		case TIMES_UP:
			beepbeep(750);
			break;
	}

}
