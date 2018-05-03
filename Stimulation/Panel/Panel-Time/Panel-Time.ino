/**
 * Panel-Time.ino
 * Receives and displays the remaining time from Max msp Serial transfer
 * Board: Arduino UNO
 */

const int PIN_BUTTON_BLACK = 4;
const int PIN_LATCH = 8;
const int PIN_CLOCK = 12;
const int PIN_DATA = 10;

const int REFRESH_CYCLE = 250;

const byte number[] = {
	B00000011, //0
	B11110011, //1
	B00100101, //2
	B01100001, //3
	B11010001, //4
	B01001001, //5
	B00001001, //6
	B11100011, //7
	B00000001, //8
	B11000001, //9
	B11111101  //-
};

String remainingTime;
boolean started;
boolean ended;
boolean buttonBlack;

void setup() {
	Serial.begin(9600);

	pinMode(PIN_BUTTON_BLACK, INPUT_PULLUP);

	// Display
	pinMode(PIN_LATCH, OUTPUT);
	pinMode(PIN_CLOCK, OUTPUT);
	pinMode(PIN_DATA, OUTPUT);

	remainingTime = "99";

	started = true;
	ended = false;
	buttonBlack = false;

}

void loop() {

	// Serial Input from Max msp
	// 1 0 60
	// started ended remainingTime

	while (Serial.available() > 0) {
		started = Serial.read();
		ended = Serial.read();
		int messageTime = Serial.read();
		String currentTime = String(messageTime);
		remainingTime = "";
		remainingTime += currentTime.charAt(0);
		remainingTime += currentTime.charAt(1);
	}

	buttonBlack = !digitalRead(PIN_BUTTON_BLACK);

	if (started) {

		if (buttonBlack && !ended) {

			int firstDigit = String(remainingTime.charAt(0)).toInt();
			int secondDigit = String(remainingTime.charAt(1)).toInt();

			digitalWrite(PIN_LATCH, LOW);

			shiftOut(PIN_DATA, PIN_CLOCK, LSBFIRST, number[firstDigit]);
			shiftOut(PIN_DATA, PIN_CLOCK, LSBFIRST, number[secondDigit]);

			digitalWrite(PIN_LATCH, HIGH);

		} else {

			digitalWrite(PIN_LATCH, LOW);

	    	shiftOut(PIN_DATA, PIN_CLOCK, LSBFIRST, B11111111);
			shiftOut(PIN_DATA, PIN_CLOCK, LSBFIRST, B11111111);

			digitalWrite(PIN_LATCH, HIGH);  
		}
	}
	
	delay(REFRESH_CYCLE);
}
