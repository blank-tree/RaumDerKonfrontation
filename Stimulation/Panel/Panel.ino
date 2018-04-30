
const int PIN_POTIS[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int PIN_STATUS_LED = 13;
const int PIN_BUTTON_RED = 2;
const int PIN_BUTTON_BLACK = 4;

int potis[8];
boolean buttonRed;
boolean buttonBlack;

unsigned long buttonRedPress;


void setup() {
	Serial.begin(9600);

	pinMode(PIN_STATUS_LED, OUTPUT);
	pinMode(PIN_BUTTON_RED, INPUT_PULLUP);
	pinMode(PIN_BUTTON_BLACK, INPUT_PULLUP);

	for(int i = 0; i < 8; i++){
	    potis[i] = 0;
	}

	buttonRed = false;
	buttonBlack = false;

	buttonRedPress = 0;

}

void loop() {

	for(int i = 0; i < 8; i++) {
		potis[i] = analogRead(PIN_POTIS[i]);
	}

	buttonBlack = digitalRead(PIN_BUTTON_BLACK);
	buttonRed = digitalRead(PIN_BUTTON_RED);

	if (!buttonRed) {
	    // turn LED on:
	    digitalWrite(PIN_STATUS_LED, HIGH);
	  } else {
	    // turn LED off:
	    digitalWrite(PIN_STATUS_LED, LOW);
	  }

	for(int i = 0; i < 8; i++) {
    	Serial.print(potis[i]);
    	Serial.print(" ");
	}
	Serial.println(buttonRed);

	delay(50);

}