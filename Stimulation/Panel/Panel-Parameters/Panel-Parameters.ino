/**
 * Panel-Parameters.ino
 * Transfer the potentiometer parameters and the start/silence
 * button to Max msp
 * Board: Arduino Mega
 */

const int PIN_POTIS[] = {A0, A1, A2, A3, A4, A8, A6, A7};
const int PIN_STATUS_LED = 13;
const int PIN_BUTTON_RED = 2;

const int REFRESH_INTERVAL_SILENCE = 1000;
const int REFRESH_CYCLE = 50;

int potis[8];

boolean started;
boolean active;
boolean ended;

boolean buttonRed;

unsigned long buttonRedPress;


void setup() {
	Serial.begin(9600);

	pinMode(PIN_STATUS_LED, OUTPUT);
	pinMode(PIN_BUTTON_RED, INPUT_PULLUP);

	for(int i = 0; i < 8; i++){
	    potis[i] = 0;
	}

	started = false;
	active = true;
	ended = false;

	buttonRed = false;

	buttonRedPress = 0;

}

void loop() {

	for(int i = 0; i < 8; i++) {
		potis[i] = analogRead(PIN_POTIS[i]);
	}

	buttonRed = !digitalRead(PIN_BUTTON_RED);

	if (!ended) {

		if (millis() > buttonRedPress + REFRESH_INTERVAL_SILENCE && buttonRed) {
			if (!started) {
				started = true;
			} else {
				active = !active;
			}
			buttonRedPress = millis();
		}

		buttonRed = digitalRead(PIN_BUTTON_RED);

		if (started && active) {
			digitalWrite(PIN_STATUS_LED, HIGH);
		} else {
			digitalWrite(PIN_STATUS_LED, LOW);
		}
	}

	// Serial Output
	// 1023 1023 1023 1023 1023 1023 1023 1023 1 1
	// poti1 poti2 poti3 poti4 poti5 poti6 poti7 poti8 started active

	for(int i = 0; i < 8; i++) {
		Serial.print(potis[i]);
		Serial.print(" ");
	}
	Serial.print(started);
	Serial.print(" ");
	Serial.println(active);

	delay(REFRESH_CYCLE);

}
