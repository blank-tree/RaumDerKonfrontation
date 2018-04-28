#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 5
#define NUMPIXELS 300
#define SERIAL_REFRESH 100

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin num^ber (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel light = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGBW + NEO_KHZ800);

int gap;
int red;
int blue;
int green;
int white;

void setup() {
	Serial.begin(9600);
	light.begin();
	light.show(); // Initialize all pixels to 'off'

	gap = 0;
	red = 0;
	blue = 0;
	green = 0;
	white = 0;
}

void loop() {

	if (Serial.available() > 0) {

		// String from Max: GAP;RED;BLUE;GREEN;WHITE;

		String instruction = Serial.readString();

		gap = getValue(instruction,' ',1).toInt();
		red = getValue(instruction,' ',2).toInt();
		blue = getValue(instruction,' ',3).toInt();
		green = getValue(instruction,' ',4).toInt();
		white = getValue(instruction,' ',5).toInt();
		
	}

	for(int i = gap ; i < NUMPIXELS - gap;  i++ ){
    	light.setPixelColor(i, 0, 0, lightValue, 0);
	}
	light.show();

	delay(SERIAL_REFRESH);
}
