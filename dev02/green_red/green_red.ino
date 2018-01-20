/* 20-Jan-2018 created
 *  
 *  Compatable with DEV02 board
 *  
 *  Perform a slow alternation between green and red LED.
 *  Speeds up whilst the button is held down.
 */
#define ATTINY85 1

constexpr int GREEN_LED = ATTINY85 ? 1 : 6; // tiny pin 6
constexpr int RED_LED = ATTINY85 ? 0 : 5; // tiny pin 5
constexpr int BUTTON = ATTINY85 ? 4 : A3; // tiny pin 3

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_LED,OUTPUT);
  digitalWrite(RED_LED, LOW);
  
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);
  
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {

  bool buttonClicked = digitalRead(BUTTON) == LOW;
  int delay_ms = buttonClicked ? 200 : 1000;

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  delay(delay_ms);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  delay(delay_ms);
  

}
