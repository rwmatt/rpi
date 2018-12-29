constexpr auto SND_SS   = 8;
constexpr auto SND_CLK  = 9;
constexpr auto SND_BYTE = 10;

void set_freq(uint16_t f) {
  digitalWrite(SND_CLK, LOW);
  digitalWrite(SND_SS, LOW);
  for(char i=0; i<16; i++) {
    digitalWrite(SND_BYTE, f> 0b0111111111111111);
    digitalWrite(SND_CLK, HIGH);
    delay(1);
    digitalWrite(SND_CLK, LOW);
    delay(1);
    f <<= 1;        
  }
  digitalWrite(SND_SS, HIGH);      
  delay(2000);
}

void setup() {
  pinMode(SND_SS, OUTPUT);
  pinMode(SND_CLK, OUTPUT);
  pinMode(SND_BYTE, OUTPUT);
  digitalWrite(SND_SS, HIGH);
  
  
}

void loop() {
  for(uint16_t f = 400; f < 4000; f += 400) {
    set_freq(f);    
  }
    
}
