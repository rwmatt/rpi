// Adapted from the Adafruit_DotStar library
// Reference is made to "SPI", but I'm sure that's
// particularly true.

// A bit-banging approach is used to talk to blinkt

const int dataPin = 4; // Blinkt pin 16
const int clockPin = 5; // blinkt pin 18
const int numLEDs = 8;
uint8_t pixels[numLEDs * 3];

void spi_out(uint8_t n) {
  for (uint8_t i = 8; i--; n <<= 1) {
    if (n & 0x80) digitalWrite(dataPin, HIGH);
    else          digitalWrite(dataPin, LOW);
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}

void show() {
  uint8_t i;
  uint8_t n = numLEDs;
  uint8_t *ptr = pixels;


  for (i = 0; i < 4; i++) spi_out(0x00); // 4 byte start-frame marker

  do {                               // For each pixel...
    spi_out(0xFF);                   //  Pixel start
    for (i = 0; i < 3; i++) spi_out(*ptr++); // Write R,G,B
  } while (--n);

  for (i = 0; i < ((numLEDs + 15) / 16); i++) spi_out(0xFF); // end-frame marker
}

void setPixelColour(uint8_t pos , uint8_t r, uint8_t g, uint8_t b) {
  
    pos *= 3;
    pixels[pos++] = b;
    pixels[pos++] = g;
    pixels[pos] = r;
}

void setup() {

  pinMode(dataPin , OUTPUT);
  pinMode(clockPin, OUTPUT);
  digitalWrite(dataPin , LOW);
  digitalWrite(clockPin, LOW);
  
  show();

}

void loop() {
  static uint8_t red = 0;
  red = 10 - red; // switch between on and off
  for (int i = 0; i < 8; ++i) {
    show();
    delay(100);
    setPixelColour(i, red, 0, 0);
  }

}
