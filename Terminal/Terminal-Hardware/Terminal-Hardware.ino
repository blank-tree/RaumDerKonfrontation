/**
   Terminal-Hardware.ino
   Transfers the amount of money payed to the iPad
   and receives the command to print the receipt
   Board: Arduino MKR1000
*/

#include <SPI.h>
#include <WiFi101.h>
#include <MQTT.h>
#include "Adafruit_Thermal.h"
#include <wiring_private.h>
#include "PlanP.h"
// #include "PlanP_E.h"
#include "RU.h"
#include "RD.h"

//Serial manuell definieren (da MKR1000 in "Adafruit_Thermal.h" nicht definiert weil neuer, nicht-arduino-mässiger Prozessor)
#define PIN_SERIAL2_RX       (1ul)                // Pin description number for PIO_SERCOM on D1
#define PIN_SERIAL2_TX       (0ul)                // Pin description number for PIO_SERCOM on D0
#define PAD_SERIAL2_TX       (UART_TX_PAD_0)      // SERCOM pad 0 TX (blau)
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_1)    // SERCOM pad 1 RX (grün)
Uart Serial2(&sercom3, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

Adafruit_Thermal printer(&Serial2);


// Settings
const int SEND_INTERVAL = 500; // 0.25s

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
volatile int moneyCollected;
boolean moneyChanged;
String language;
String appointmentDate;
String appointmentTime;
boolean currentlyPrinting;

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
    language = payload;
  } else if (topic == "/appointmentDate") {
    appointmentDate = payload;
  } else if (topic == "/appointmentTime") {
    appointmentTime = payload;
  } else if (topic == "/print") {
    printReceipt();
  } else if (topic == "/resettvm") {
    // nothing
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
  currentlyPrinting = false;

  pinMode(6, INPUT);
  digitalWrite(6, LOW);

  pinMode(PIN_COINS, INPUT_PULLUP);
  attachInterrupt(PIN_COINS, coinInserted, RISING);

  pinMode(PIN_PRINTER, OUTPUT);
  digitalWrite(PIN_PRINTER, LOW);
  Serial2.begin(19200);
  pinPeripheral(0, PIO_SERCOM);   // Assign pins 0 & 1 SERCOM functionality
  pinPeripheral(1, PIO_SERCOM);
  printer.begin();  // Init printer (same regardless of serial type)
  printer.sleep();

}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  if (moneyChanged && millis() - lastMillis > SEND_INTERVAL && !currentlyPrinting) {
    lastMillis = millis();
    moneyChanged = false;
    Serial.println(String(moneyCollected));
    client.publish("/moneyCollected", String(moneyCollected));
  }
}

void coinInserted() {
  moneyCollected = moneyCollected + 10;
  moneyChanged = true;  
}

void printReceipt() {

  client.disconnect();


  currentlyPrinting = true;
  int appointmentEnding = appointmentTime != "" ? appointmentTime.toInt() + 1 : 0;
  String appointmentEndingString = appointmentEnding <= 9 ? "0" + String(appointmentEnding) : String(appointmentEnding);

  printer.wake();       // MUST wake() before printing again, even if reset
  printer.setDefault(); // Restore printer to defaults


  if (language == "de") {
    //Deutsch ***********************************************************************************
    //R up
    printer.justify('C');
    printer.printBitmap(RU_width, RU_height, RU_data);
    
    printer.justify('C');
    printer.println(("\n \n"));

    //Raum der Konfrontation
    printer.boldOn();
    printer.setSize('M');
    printer.justify('C');
    printer.println(("\n "));
    printer.println(("\nRaum der Konfrontation"));
    printer.boldOff();

    //Ticket berechtigt zum Benuetzen des Raum waehrend diesem Zeitfenster:
    printer.setSize('S');
    printer.justify('C');
    printer.println(("\n \nTicket berechtigt zum\nBenuetzung des Raum waehrend \ndieses Zeitraums:"));

    //Datum und Zeit
    printer.boldOn();
    printer.setSize('M');
    printer.justify('C');
    printer.println("\nDatum: " + appointmentDate + ".Juni.2018\nZeit: " + appointmentTime + ":00 - " + appointmentEndingString + ":00\nOrt: 5.T04 (ZHdK/Turm)");
    printer.boldOff();

    //Der Raum wird 10 Minuten vor Beginn geöffnet. 10 Minuten nach Schluss erscheint das Putzpersonal.
    printer.setSize('S');
    printer.justify('C');
    printer.println(("\nDer Raum wird 10 Minuten vor \nBeginn geoeffnet. \n10 Minuten nach Ende \nerscheint das Putzpersonal.\n \n"));

    //Plan Weg Raum
    printer.justify('C');
    printer.printBitmap(PlanP_width, PlanP_height, PlanP_data);

    //Webseite
    printer.justify('C');
    printer.println(("\n \nwww.raumderkonfrontation.com\n\n"));

    //R down
    printer.justify('C');
    printer.printBitmap(RD_width, RD_height, RD_data);

    printer.justify('C');
    printer.println(("\n\n\n\n\n\n"));
  }


  else {
    //English ***********************************************************************************
    //R up
    printer.justify('C');
    printer.printBitmap(RU_width, RU_height, RU_data);

    printer.justify('C');
    printer.println(("\n \n"));

    //Raum der Konfrontation
    printer.boldOn();
    printer.setSize('S');
    printer.justify('C');
    printer.println(("\n "));
    printer.println(("\nRoom of Confrontation"));
    printer.boldOff();

    //Ticket berechtigt zum Benuetzen des Raum waehrend diesem Zeitfenster:
    printer.setSize('S');
    printer.justify('C');
    printer.println(("\n \nTicket is valid to\nuse the room during \nthis period:"));

    //Datum und Zeit
    printer.boldOn();
    printer.setSize('M');
    printer.justify('C');
    printer.println("\nDate: " + appointmentDate + ".June.2018\nTime: " + appointmentTime + ":00 - " + appointmentEndingString + ":00\nOrt: 5.T04 (ZHdK/Tower)");
    printer.boldOff();

    //Der Raum wird 10 Minuten vor Beginn geöffnet. 10 Minuten nach Schluss erscheint das Putzpersonal.
    printer.setSize('S');
    printer.justify('C');
    printer.println(("\nThe room will be opened \n10 minutes before beginning. \n10 minutes after the end \nthe cleaning staff will arive.\n \n"));

    //Plan Weg Raum
    printer.justify('C');
    printer.printBitmap(PlanP_width, PlanP_height, PlanP_data);

    //Webseite
    printer.justify('C');
    printer.println(("\n \nwww.raumderkonfrontation.com\n\n"));

    //R down
    printer.justify('C');
    printer.printBitmap(RD_width, RD_height, RD_data);

    printer.justify('C');
    printer.println(("\n\n\n\n\n\n"));

    // ***********************************************************************************
  }


  printer.feed(4);//default is 2
  printer.setDefault(); // Restore printer to defaults
  printer.sleep();      // Tell printer to sleep
  //delay(3000L);         // Sleep for 3 seconds
  // printer.wake();       // MUST wake() before printing again, even if reset
  
  moneyCollected = 0;
  appointmentDate = "";
  appointmentTime = "";
  currentlyPrinting = false;
  
   pinMode(6, OUTPUT);   // lights out. Assuming it is jumper-ed correctly.
   while(1);

}
