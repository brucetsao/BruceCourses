#include <LWiFi.h>
#include <LWiFiClient.h>
#define SITE_URL "184.106.153.149"
#define WIFI_AP "linkitone" //請輸入所要連的AP名稱
#define WIFI_PWD "" // 請輸入AP 密碼
#define SensorPin A1
LWiFiClient client;
// ThingSpeak Settings
String writeAPIKey = "7FQZ6JEDHQO6QF1H ";    // Write API Key for a ThingSpeak Channel
const int updateInterval = 5000;        // Time interval in milliseconds to update ThingSpeak   

// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int resetCounter = 0;


void setup() {
 Serial.begin(9600);
  while(!Serial) ;
 LWiFi.begin();

 Serial.println();
 Serial.print("Connecting to AP...");
// if(LWiFi.connectWEP(WIFI_AP, WIFI_PWD) < 0)
 if(LWiFi.connect(WIFI_AP) < 0)
 {
 Serial.println("FAIL!");
 return;
 }
 Serial.println("ok");
 Serial.print("Connecting to site...");
 if(!client.connect(SITE_URL, 80))
 {
 Serial.println("FAIL!");
 return;
 }
 Serial.println("ok");
 //client.close() ;
}

//String analogValue0 = String(lightMeter.readLightLevel(), DEC);
void loop()
{
  String analogdata = String(analogRead(SensorPin), DEC);
  Serial.print("Data is :(");
  Serial.print(analogdata);
  Serial.print(")\n");
  
  // Print Update Response to Serial Monitor
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }
  
  // Disconnect from ThingSpeak
  if (!client.connected() && lastConnected)
  {
    Serial.println();
    Serial.println("...disconnected.");
    Serial.println();
    
    client.stop();
  }
  
  // Update ThingSpeak
  if(!client.connected() && (millis() - lastConnectionTime > updateInterval))
  {
    updateThingSpeak("field4="+analogdata);
    Serial.println("Update thingspeak is ok");
  }
  
  lastConnected = client.connected();
  delay(10000) ;
}
//===============
void updateThingSpeak(String tsData)
{
  if (client.connect(SITE_URL, 80))
  { 
    Serial.println("Connected to ThingSpeak...");
    Serial.println();
        
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    
    lastConnectionTime = millis();
    
    resetCounter = 0;
    
  }
  else
  {
    Serial.println("Connection Failed.");   
    Serial.println();
    
    resetCounter++;
    
    if (resetCounter >=5 ) {resetEthernetShield();}

    lastConnectionTime = millis(); 
  }
}

void resetEthernetShield()
{
  Serial.println("Resetting Ethernet Shield.");   
  Serial.println();
  
  client.stop();
  //delay(1000);
  
 // Ethernet.begin(mac, ip, gateway, subnet);
  delay(1000);
}





