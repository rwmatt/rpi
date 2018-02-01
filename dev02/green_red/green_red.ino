/* 20-Jan-2018 created
 *  
 *  Compatable with DEV02 board
 *  
 *  Provides a variety of patterns
 *  
 */
#define ATTINY85 1

constexpr int GREEN_LED = ATTINY85 ? 1 : 6; // tiny pin 6
constexpr int RED_LED = ATTINY85 ? 0 : 5; // tiny pin 5
constexpr int BUTTON = ATTINY85 ? 4 : A3; // tiny pin 3

// convenience function
void show(bool green_on, bool red_on, long int show_for_ms) {
  digitalWrite(GREEN_LED, green_on ? LOW : HIGH);
  digitalWrite(RED_LED, red_on ? LOW : HIGH);
  delay(show_for_ms);
}
void pattern1() {
 /*
 *  Perform a slow alternation between green and red LED.
 *  Speeds up whilst the button is held down.
 */

  // setup
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  while(1) {
    bool buttonClicked = digitalRead(BUTTON) == LOW;
    int delay_ms = buttonClicked ? 200 : 1000;

    // TODO use the show() function
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      delay(delay_ms);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      delay(delay_ms);    
  }
}

void blinkAll(long int duration) {
  while(1) {
    show(false, false, duration);
    show(true, true, duration);
  }
}

void pattern2() { blinkAll(500); }
void pattern3() { blinkAll(2000); }

void pattern4() { // alternate
  while(1) {
    show(true, false, 500);
    show(false, true, 500);
  }
}

void setup() {
  pinMode(RED_LED,OUTPUT);  
  pinMode(GREEN_LED, OUTPUT);  
  pinMode(BUTTON, INPUT_PULLUP);

  // select one of the patterns
  pattern4();
}

void loop() {

// main loop is within the pattern itself
  

}
