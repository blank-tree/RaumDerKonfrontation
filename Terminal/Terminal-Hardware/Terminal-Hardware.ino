#include <SPI.h>
#include <WiFi101.h>
#include <MQTT.h>

// Settings


// Shitr.io Connection
const char ssid[] = "BRIDGE";
const char pass[] = "internet";
WiFiClient net;
MQTTClient client;
unsigned long lastMillis = 0;

// Functionality
int moneyCollected;
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
  client.subscribe("/resettvm");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  if (topic == "/language") {
    Serial.println("Language is " + payload);
  } else if (topic == "/appointmentDate") {
    Serial.println("The date of the appointment is " + payload);
  } else if (topic == "/appointmentTime") {
    Serial.println("The time of the appointment is " + payload);
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
  language = "";
  appointmentDate = "";
  appointmentTime = "";
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    moneyCollected++;
    client.publish("/moneyCollected", String(moneyCollected));
  }
}

void resetTVM() {
  moneyCollected = 0;
  appointmentDate = "";
  appointmentTime = "";
}