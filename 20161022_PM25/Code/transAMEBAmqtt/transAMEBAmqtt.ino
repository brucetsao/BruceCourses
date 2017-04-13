/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <WiFi.h>

char ssid[] = "Microwind_TWN";     //  your network SSID (name)
char pass[] = "0919734011";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

#include <PubSubClient2.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 172, 16, 0, 2 };
byte ip[]     = { 172, 16, 0, 100 };

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

WiFiClient wifiClient;
PubSubClient client("gpssensor.ddns.net", 1883, callback, wifiClient);

void setup()
{
  Serial.begin(115200);
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  if (client.connect("arduinoClient")) {
    client.subscribe("inTopic");
  }
}

void loop()
{
  client.publish("LASS/Test/Farm","Valeu=100");
  delay(1000);
  Serial.println("LOOPS");
}

