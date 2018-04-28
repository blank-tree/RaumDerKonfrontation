int poti1 = 0;
int poti2 = 0;
int poti3 = 0;
int poti4 = 0;
int poti5 = 0;
int poti6 = 0;
int poti7 = 0;
int poti8 = 0;
boolean buttonBlack = false;
boolean buttonRed = false;


void setup() {
//set baud rate
Serial.begin(9600);

pinMode(13, OUTPUT);
pinMode(4, INPUT_PULLUP);
pinMode(2, INPUT_PULLUP);
}

void loop() {
//read potis
poti1 = analogRead(A0);
poti2 = analogRead(A1);
poti3 = analogRead(A2);
poti4 = analogRead(A3);
poti5 = analogRead(A4);
poti6 = analogRead(A5);
poti7 = analogRead(A6);
poti8 = analogRead(A7);

buttonBlack = digitalRead(4);
buttonRed = digitalRead(2);

if (!buttonRed) {
    // turn LED on:
    digitalWrite(13, HIGH);
  } else {
    // turn LED off:
    digitalWrite(13, LOW);
  }

//Send values
Serial.print(poti1);
Serial.print(" ");
Serial.print(poti2);
Serial.print(" ");
Serial.print(poti3);
Serial.print(" ");
Serial.print(poti4);
Serial.print(" ");
Serial.print(poti5);
Serial.print(" ");
Serial.print(poti6);
Serial.print(" ");
Serial.print(poti7);
Serial.print(" ");
Serial.print(poti8);
Serial.print(" ");
Serial.println(buttonRed);
delay(50);

}
