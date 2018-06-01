import mqtt.*;
import processing.serial.*;

MQTTClient client;
Serial myPort;

void setup() {
  client = new MQTTClient(this);
  client.connect("mqtt://b23695cf:36a044b175c04e97@broker.shiftr.io", "RdK-Terminal");
  client.subscribe("/language");
  client.subscribe("/appointmentDate");
  client.subscribe("/appointmentTime");
  client.subscribe("/print");
  client.subscribe("/resettvm");
  
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[7], 9600);

}

void draw() {
	while (myPort.available() > 0) {
		client.publish("/moneyCollected", str(myPort.read())); 
	}
}

void messageReceived(String topic, byte[] payload) {
  println(topic + ":" + new String(payload));
}
