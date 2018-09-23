#define I2C_SLAVE_ADDRESS 0x4

#include <TinyWireS.h>
int i=0;

void setup()
{
  TinyWireS.begin(I2C_SLAVE_ADDRESS);
  TinyWireS.onRequest(requestEvent);
  pinMode(PB4, 1);
  digitalWrite(PB4, HIGH);
}

void loop()
{
  TinyWireS_stop_check();
}

void requestEvent()
{
  TinyWireS.send(i);
  i++;  
}
