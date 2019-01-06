/*
 * Sketch for testing sleep mode with wake up on WDT.
 * Donal Morrissey - 2011.
 * 
 * http://donalmorrissey.blogspot.com/2010/04/sleeping-arduino-part-5-wake-up-via.html
 *
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

constexpr auto led = PB4; // pin 4 on Ardunio; pin 3 on tiny85
//static_assert(PB0 == 14, "unexpected pin numbering");

//#define LED_PIN (13)

//volatile int f_wdt=1;



/***************************************************
 *  Name:        ISR(WDT_vect)
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Watchdog Interrupt Service. This
 *               is executed when watchdog timed out.
 *
 ***************************************************/

 // You need to have this, whether you use it or not. Otherwise the device will reset
ISR(WDT_vect)
{
  /*
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
  else
  {
    Serial.println("WDT Overrun!!!");
  }
  */
}

#if defined(__AVR_ATtiny85__)
#pragma message "Using watchdog register for ATTiny85"
#define WDTCSR WDTCR
#endif

void initSleep()
{

  /*** Setup the WDT ***/
  
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
}

/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Enters the arduino into sleep mode.
 *
 ***************************************************/
void enterSleep(void)
{
  //set_sleep_mode(SLEEP_MODE_PWR_SAVE);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  
  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();
}



/***************************************************
 *  Name:        setup
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Setup for the serial comms and the
 *                Watch dog timeout. 
 *
 ***************************************************/
void setup()
{
  Serial.begin(9600);
  Serial.println("Initialising...");
  //Serial.println(PB1, DEC);
  delay(100); //Allow for serial print to complete.

  pinMode(led,OUTPUT);
  initSleep();
  
  Serial.println("Initialisation complete.");
  delay(100); //Allow for serial print to complete.
}



/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Main application loop.
 *
 ***************************************************/

void test() {

  // a counter
  static int i = 0;
  Serial.println(i, DEC);
  Serial.flush();
  i++;  

  // a blinker
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);  

}

/*
void test2() {
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);  
}
*/

void loop()
{
  test();
  enterSleep();
}


void yuk() {
  

  
  static int i = 0;
  //if(f_wdt == 1)
  {
    /* Toggle the LED */
    //digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    Serial.println(i, DEC);
    Serial.flush();
    i++;
    
    /* Don't forget to clear the flag. */
    //f_wdt = 0;
    
    /* Re-enter sleep mode. */
    enterSleep();
  }
  //else
  {
    /* Do nothing. */
  }
}
