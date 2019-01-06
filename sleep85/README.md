# sleep85

Putting the ATTiny85 to sleep for 8 seconds, conserving a lot of power in the process.

A blank sketch uses 1.3mA. Using the sleep85 code, the power draw is 0.3mA when the LED is off. Using the sketch in bak, I was able to get the power draw down to 5uA. However, the board seemed to reset, losing the value of the variables, and starting setup() again.

Adding an ISR(WDT_vect) seems crucial to stopping the board reset, so the sketch in bak may yet have more mileage to it.