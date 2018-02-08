/*
 * Started 08-Feb-2018
 * 
 * Meditation timer
 * 
 * You probably need the clock running at 8MHz
 * 
 * See my diary notes of 08.2.18, which contains a schematic.
 * 
 * Suggested Dupont connections on my mini-board:
 * 1 GND  brown
 * 2 5V   red
 * 3 DP9  orange
 * 4 DP12 yellow
 * 5 A0   green
 * Pins are labelled from top to bottom, with pin-out on LHS
 * 
 * Potentiometer seems busted
 */
 
//#define F_CPU 8000000L

unsigned long startTime;
constexpr int buzzerPin = 9;
constexpr int ledPin = 12;
constexpr int potPin = A0;

constexpr unsigned long mins30 = 30L * 60L * 1000L; // 30 minutes
enum state {RUNNING, TIMES_UP} the_state;

void setup()
{
  startTime = millis();
  the_state = RUNNING;
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(potPin, INPUT);
  //digitalWrite(buzzer, HIGH);
  Serial.begin(9600);
}

void beep(int postDelay)
{
  //digitalWrite(buzzer, HIGH);
  int potVal = analogRead(potPin);
  int vol = map(potVal, 0, 1023, 0, 255);
  vol = 100; // pot seems busted
  Serial.println(vol);
  analogWrite(buzzerPin, vol);
  delay(500);
  digitalWrite(buzzerPin, LOW);
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
			beepbeep(500);
      digitalWrite(ledPin, HIGH);
			break;
	}

}


