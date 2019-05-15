#include <Wire.h>

#include "mal.h"

const uint8_t addr = 0x70; // HT16K33 default address
constexpr int btnSmiley = 2; // D2

Every ev{1000}; 

static const uint8_t  smile[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 };

void send_cmd(uint8_t cmd) {
  Wire.beginTransmission(addr);
  Wire.write(cmd);
  Wire.endTransmission();
}
    
void setup() {
  Wire.begin();
  send_cmd(0x20 | 1); // turn on oscillator
  send_cmd(0x81); // display on
  send_cmd(0xE0 | 0); // brightness to dimmest (but you should probably set it)

  pinMode(btnSmiley, INPUT_PULLUP);
  randomise_bitmap();
}


void write_row(uint8_t y, uint8_t xs){
  // the row of xs beed to be quirkily transformed
  uint8_t bits = 0;
  for(int x = 0; x<7; ++x) {
      bits <<= 1;
      bits |= (xs & 1);
      xs  >>= 1;      
    }
    bits |= (xs << 7);

    // send x layout to device
    Wire.beginTransmission(0x70); // address of LED matrix
    Wire.write(2*y);
    Wire.write(bits);
    Wire.endTransmission();
}

uint8_t random_bitmap[8];
  
void randomise_bitmap() {
  for(int y = 0; y<8; ++y)  random_bitmap[y] = random(0, 0xFF);
}


void loop() {
  if(ev.rising()) randomise_bitmap();
  bool show_smiley = digitalRead(btnSmiley) == LOW;

  // display pattern
  for(int y = 0; y<8; ++y) {
    uint8_t xs = show_smiley ? smile[y] : random_bitmap[y];
      write_row(y, xs);   
  }
}
