/**
 * Terminal-Hardware.ino
 * Transfers the amount of money payed to the iPad 
 * and receives the command to print the receipt
 * Board: MKR1000
 */

#include <SPI.h>
#include <WiFi101.h>
#include <MQTT.h>

// Settings
const boolean PRINTING = false;
const boolean COINS = false;

// Pins
const int PIN_COINS = 0;

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
    Serial.println("Language is " + payload);
    language = payload;
  } else if (topic == "/appointmentDate") {
    Serial.println("The date of the appointment is " + payload);
    appointmentDate = payload;
  } else if (topic == "/appointmentTime") {
    Serial.println("The time of the appointment is " + payload);
    appointmentTime = payload;
  } else if (topic == "/print") {
    Serial.println("Printing the receipt");
    printReceipt();
  } else if (topic == "/resettvm") {
    Serial.println("Reset the whole thing ");
    resetTVM();
  }

}

void setup() {
  Serial.begin(115200);
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
  
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (moneyChanged && millis() - lastMillis > 1000) {
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
    // Thermal printer code
  } else {
    // Testing code
  }
}

void resetTVM() {
  moneyCollected = 0;
  appointmentDate = "";
  appointmentTime = "";
}