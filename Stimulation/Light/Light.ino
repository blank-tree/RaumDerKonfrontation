#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 5
#define NUMPIXELS 300

int ledValues[6] = {0,0,0,0,0,0};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGBW + NEO_KHZ800);


void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {

  bool change = false;
  String serialReceived = "";
  int loopIterator = 0;
  
  while (Serial.available() > 0) {

    change = true;
    if (loopIterator < 6) {
      ledValues[loopIterator] = Serial.read();
      serialReceived += ledValues[loopIterator];
      loopIterator++;
      }
    
  }

  if (change) {    
    Serial.println(serialReceived);
    for (int i = 0; i < NUMPIXELS; i++) {
      if (i > ledValues[1] && i < NUMPIXELS - ledValues[1]) {
          strip.setPixelColor(i, ledValues[2], ledValues[3], ledValues[4], ledValues[5]);
        } else {
          strip.setPixelColor(i, 0, 0, 0, 0);
          }
      
      }
    strip.show();
  }

}
