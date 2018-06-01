import mqtt.*;
import processing.serial.*;

final static int SEND_INTERVAL = 800; // 0.8sec

long lastInterval;
MQTTClient client;
Serial arduinoPort;
float moneyCollected;

void setup() {
  client = new MQTTClient(this);
  client.connect("mqtt://b23695cf:36a044b175c04e97@broker.shiftr.io", "RdK-Terminal");
  client.subscribe("/language");
  client.subscribe("/appointmentDate");
  client.subscribe("/appointmentTime");
  client.subscribe("/print");
  client.subscribe("/resettvm");
  
  printArray(Serial.list());
  arduinoPort = new Serial(this, Serial.list()[7], 9600);

  lastInterval = 0;
  moneyCollected = 0.0;

}

void draw() {
  while (arduinoPort.available() > 0) {

    String inBuffer = arduinoPort.readString();

    if (inBuffer != null) {
      inBuffer = inBuffer.replaceAll("\\D+","");
      
      if (millis() > lastInterval + SEND_INTERVAL) {
        client.publish("/moneyCollected", inBuffer);
        lastInterval = millis();
      }
    }
  }
}

void messageReceived(String topic, byte[] payload) {
  println(topic + ":" + new String(payload));
}