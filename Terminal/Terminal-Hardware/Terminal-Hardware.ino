/**
 * Terminal-Hardware.ino
 * Transfers the amount of money payed to the iPad 
 * and receives the command to print the receipt
 * Board: MKR1000
 */

#include <SPI.h>
#include <WiFi101.h>
#include <MQTT.h>
#include "Adafruit_Thermal.h"
#include "Plan2.h"
#include "R_up.h"
#include "R_down.h"

//Serial manuell definieren (da MKR1000 in "Adafruit_Thermal.h" nicht definiert weil neuer, nicht-arduino-mässiger Prozessor)
#define PIN_SERIAL2_RX       (1ul)                // Pin description number for PIO_SERCOM on D1
#define PIN_SERIAL2_TX       (0ul)                // Pin description number for PIO_SERCOM on D0
#define PAD_SERIAL2_TX       (UART_TX_PAD_0)      // SERCOM pad 0 TX (blau)
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_1)    // SERCOM pad 1 RX (grün)
Uart Serial2(&sercom3, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

Adafruit_Thermal printer(&Serial2);

// Settings
const boolean PRINTING = true;
const boolean COINS = true;
const int SEND_INTERVAL = 800; // 0.8s

// Pins
const int PIN_COINS = 4;
const int PIN_PRINTER = 7;

// Shitr.io Connection
const char ssid[] = "BRIDGE";
const char pass[] = "internet";
WiFiClient net;
MQTTClient client;
unsigned long lastMillis = 0;

// Functionality
volatile float moneyCollected;
boolean moneyChanged;
String language;
String appointmentDate;
String appointmentTime;


//Funktion für den Serial2
void SERCOM3_Handler() {   // Interrupt handler for SERCOM3
  Serial2.IrqHandler();
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("RdK-Arduino", "b23695cf", "36a044b175c04e97")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/language");
  client.subscribe("/appointmentDate");
  client.subscribe("/appointmentTime");
  client.subscribe("/print");
  client.subscribe("/resettvm");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  if (topic == "/language") {
    // Serial.println("Language is " + payload);
    language = payload;
  } else if (topic == "/appointmentDate") {
    // Serial.println("The date of the appointment is " + payload);
    appointmentDate = payload;
  } else if (topic == "/appointmentTime") {
    // Serial.println("The time of the appointment is " + payload);
    appointmentTime = payload;
  } else if (topic == "/print") {
    // Serial.println("Printing the receipt");
    printReceipt();
  } else if (topic == "/resettvm") {
    // Serial.println("Reset the whole thing ");
    resetTVM();
  }

}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();

  moneyCollected = 0;
  moneyChanged = false;
  language = "";
  appointmentDate = "";
  appointmentTime = "";

  if (COINS) {
    pinMode(PIN_COINS, INPUT_PULLUP);
    attachInterrupt(PIN_COINS, coinInserted, RISING);
  }

  if (PRINTING) {
    pinMode(PIN_PRINTER, OUTPUT);
    digitalWrite(PIN_PRINTER, LOW);
    Serial2.begin(9600);
    pinPeripheral(0, PIO_SERCOM);   // Assign pins 0 & 1 SERCOM functionality
    pinPeripheral(1, PIO_SERCOM);
    printer.begin();  // Init printer (same regardless of serial type)
  }
  
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (moneyChanged && millis() - lastMillis > SEND_INTERVAL) {
    lastMillis = millis();
    moneyChanged = false;
    client.publish("/moneyCollected", String(moneyCollected));
  }
}

void coinInserted() {
  moneyCollected++;
  moneyChanged = true;
}

void printReceipt() {
  if (PRINTING) {
    int appointmentEnding = appointmentTime.toInt() + 1;
    String appointmentEndingString = appointmentEnding <= 9 ? "0" + String(appointmentTime) : String(appointmentTime);
    printer.justify('C');
    printer.println(F("...................\n "));

    //R up
    printer.justify('C');
    printer.printBitmap(R_up_width, R_up_height, R_up_data);

    //Raum der Konfrontation
    printer.boldOn();
    printer.setSize('S');
    printer.justify('C');
    printer.println(F(" \nRaum der Konfrontation"));
    printer.boldOff();

    //Ticket berechtigt zum Benuetzen des Raum waehrend diesem Zeitfenster:
    printer.setSize('S');
    printer.justify('C');
    printer.println(F(" \n  \nTicket berechtigt zum\nBenuetzung des Raum waehrend \ndieses Zeitraums:"));

    //Datum und Zeit
    printer.boldOn();
    printer.setSize('M');
    printer.justify('C');
    printer.println(F(" \nDatum: " + appointmentDate + ".Juni.2018\nZeit: " + appointmentTime + ":00 - " + appointmentEndingString + ":00\nOrt: 5.T04 (ZHdK/Turm)"));
    printer.boldOff();

    //Der Raum wird 10 Minuten vor Beginn geöffnet. 10 Minuten nach Schluss erscheint das Putzpersonal.
    printer.setSize('S');
    printer.justify('C');
    printer.println(F(" \nDer Raum wird 10 Minuten vor \nBeginn geoeffnet. \n10 Minuten nach Ende \nerscheint das Putzpersonal.\n \n"));

    //Plan Weg Raum
    printer.justify('C');
    printer.printBitmap(Plan2_width, Plan2_height, Plan2_data);

    //Webseite
    printer.justify('C');
    printer.println(F(" \n\nwww.raumderkonfrontation.com\n \n "));
    printer.justify('C');
    printer.println(F("..................."));

    //R down
    printer.justify('C');
    printer.printBitmap(R_down_width, R_down_height, R_down_data);

    printer.feed(8);//default is 2

    printer.sleep();      // Tell printer to sleep
    delay(3000L);         // Sleep for 3 seconds
    printer.wake();       // MUST wake() before printing again, even if reset
    printer.setDefault(); // Restore printer to defaults
  }
}

void resetTVM() {
  moneyCollected = 0;
  appointmentDate = "";
  appointmentTime = "";
}