
const int PIN_POTIS[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int PIN_STATUS_LED = 13;
const int PIN_BUTTON_RED = 2;
const int PIN_BUTTON_BLACK = 4;
const int PIN_LATCH = 9;
const int PIN_CLOCK = 13;
const int PIN_DATA = 11;

const int REFRESH_INTERVAL_PAUSE = 200;

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

int potis[8];

boolean active;
String remainingTime;

boolean buttonRed;
boolean buttonBlack;

unsigned long buttonRedPress;


void setup() {
	Serial.begin(9600);

	pinMode(PIN_STATUS_LED, OUTPUT);
	pinMode(PIN_BUTTON_RED, INPUT_PULLUP);
	pinMode(PIN_BUTTON_BLACK, INPUT_PULLUP);

	// Display
	pinMode(PIN_LATCH, OUTPUT);
	pinMode(PIN_CLOCK, OUTPUT);
	pinMode(PIN_DATA, OUTPUT);

	for(int i = 0; i < 8; i++){
	    potis[i] = 0;
	}

	active = false;
	remainingTime = "42";

	buttonRed = false;
	buttonBlack = false;

	buttonRedPress = 0;

}

void loop() {

	for(int i = 0; i < 8; i++) {
		potis[i] = analogRead(PIN_POTIS[i]);
	}

	while (Serial.available() > 0) {
		String incomingMessage = String(Serial.read());
		active = String(incomingMessage.charAt(0)).toInt();
		remainingTime = "";
		remainingTime += incomingMessage.charAt(1);
		remainingTime += incomingMessage.charAt(2);
	}

	buttonRed = !digitalRead(PIN_BUTTON_RED);
	buttonBlack = !digitalRead(PIN_BUTTON_BLACK);

	if (buttonBlack) {

		int firstDigit = String(remainingTime.charAt(0)).toInt();
		int secondDigit = String(remainingTime.charAt(1)).toInt();

		digitalWrite(PIN_LATCH, LOW);

		shiftOut(PIN_DATA, PIN_CLOCK, LSBFIRST, number[firstDigit]);
		shiftOut(PIN_DATA, PIN_CLOCK, LSBFIRST, number[secondDigit]);

		digitalWrite(PIN_LATCH, HIGH);
	}

	if (millis() > buttonRedPress + REFRESH_INTERVAL_PAUSE && buttonRed) {
		active = !active;
		buttonRedPress = millis();
	}

	buttonRed = digitalRead(PIN_BUTTON_RED);

	if (active) {
		digitalWrite(PIN_STATUS_LED, HIGH);
	} else {
		digitalWrite(PIN_STATUS_LED, LOW);
	}

	for(int i = 0; i < 8; i++) {
    	Serial.print(potis[i]);
    	Serial.print(" ");
	}
	Serial.println(active);

	delay(50);

}