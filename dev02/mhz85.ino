//#pragma optimize("", off);

#define ATTINY85 1

constexpr int GREEN_LED = ATTINY85 ? 1 : 6; // tiny pin 6
constexpr int RED_LED = ATTINY85 ? 0 : 5; // tiny pin 5
constexpr int BUTTON = ATTINY85 ? 4 : A3; // tiny pin 3

enum  { WAITING, COMPUTING } the_state;

void off(int led) { digitalWrite(led, LOW); }
void on(int led) { digitalWrite(led, HIGH); }

void setup() {
  the_state = WAITING;

  pinMode(RED_LED,OUTPUT);
  on(RED_LED);
    
  pinMode(GREEN_LED, OUTPUT);
  off(GREEN_LED);
  
  pinMode(BUTTON, INPUT_PULLUP);
}

bool buttonClicked() { return digitalRead(BUTTON) == LOW; }

void doIntensiveCalculations()
{
  int sum = 0;
  constexpr int k1 = 1000;
  constexpr int k10 = 10 * k1;

  // ints don't go up this far
  //int _100k = 100 * _1k;
  //int _1m = _1k * _1k;
  
  for(int i = 0 ; i < 3 ; i++) 
    for(int j = 0 ; j < 100 ; j++)
      for(int k = 0 ; k < 100 ; k++) 
        sum += random(1,5);
      

  // defeat optimisation
  if(sum) delay(1);     
  
}
void loop() {

  switch(the_state) {
    case WAITING:
      if(buttonClicked()) the_state = COMPUTING;
      break;
    case COMPUTING:
      on(GREEN_LED);
      off(RED_LED);
      doIntensiveCalculations();
      off(GREEN_LED);
      on(RED_LED);
      while(buttonClicked()) delay(50); // wait for button to be released if it is still presses
      the_state = WAITING;
      break;
  }


}
