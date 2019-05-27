#include <SPI.h>

constexpr int rs_pin = 12;
constexpr int cs_pin = 10;

/* rs_val = LOW to send command, HIGH to send ASCII char 
 */
void sendByte(int rs_val, int val) {
  digitalWrite(rs_pin, rs_val);
  digitalWrite(cs_pin, LOW);
  SPI.transfer(val);
  digitalWrite(cs_pin, HIGH);  
  delay(60);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(cs_pin, OUTPUT);
  digitalWrite(cs_pin, HIGH);
  pinMode(rs_pin, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  Serial.begin(9600);

  constexpr auto contrast = 0x70  | 0b1000; // from 0x7C
  constexpr auto display = 0b1111; // ori 0x0F
  uint8_t cmds[] = {0x39, 0x1D, 0x50, 0x6C, contrast , 0x38, display, 0x01, 0x06};
  for(int i=0; i<sizeof(cmds); ++i) sendByte(LOW, cmds[i]);
  Serial.println(sizeof(cmds));

  // now send some intersting output
  uint8_t msg[] = {'H', 'E', 'L', 'L', 'O'};
  for(int i=0; i<sizeof(msg); ++i) sendByte(HIGH, msg[i]);
  Serial.println(sizeof(msg));


}


void loop() {
  // put your main code here, to run repeatedly:
  delay(10);

}
