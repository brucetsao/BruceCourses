#include <LDateTime.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#define SITE_URL "184.106.153.149"      //ThingSpeak WebSite IP address
//#define WIFI_AP "linkitone" //請輸入所要連的AP名稱
//#define WIFI_AP "BruceTsaoiPhone5" //請輸入所要連的AP名稱
#define WIFI_AP "linkitone" //請輸入所要連的AP名稱
#define WIFI_PWD "" // 請輸入AP 密碼
#define SensorPin A1    //Detect Current Data
#define RelayPin 13        //Relay Contrtol pin
#define turnon HIGH        
#define turnoff LOW


LWiFiClient client;
// ThingSpeak Settings
String writeAPIKey = "7FQZ6JEDHQO6QF1H ";    // Write API Key for a ThingSpeak Channel
const int updateInterval = 5000;        // Time interval in milliseconds to update ThingSpeak   
//data object
datetimeInfo t;

// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int resetCounter = 0;
double MidValue = 524 ;
char getdata ;  // 儲存接收資料的變數


void setup() {
    pinMode(RelayPin , OUTPUT) ;      //set relay control pin as pin 8
      pinMode(Aout,INPUT);      //set analog pin1 as input
 Serial.begin(9600);    // monitor baud rate
 Serial1.begin(9600) ;  // bluetooth baud rate
  while(!Serial) ;    //waiting for open Serial monitor window
 LWiFi.begin();    // init Wifi function

 Serial.println();    //send new line
 Serial.print("Connecting to AP...");    //send message
// if(LWiFi.connectWEP(WIFI_AP, WIFI_PWD) < 0)      //connect AP with WEP
 //if(LWiFi.connect(WIFI_AP) < 0)       //connect AP without WEP
 if(LWiFi.connect(WIFI_AP) < 0)        //fail init wifi  
 {
 Serial.println("FAIL!");
 return;
 }
 Serial.println("Wifi is ok");
 delay(2000);    // waiting wifi ready
 Serial.print("Now Connecting to site...");      //
 if(!client.connect(SITE_URL, 80))    //Now connecting WEBSITE (SITE_URL)
 {
 Serial.println("FAIL!");
 return;      // fail connect website
 }
 Serial.println("ok");
 //client.close() ;
}

//String analogValue0 = String(lightMeter.readLightLevel(), DEC);
void loop()
{
      getdata = '@'; 
  // 若收到藍牙模組的資料，則送到「序列埠監控視窗」
  if (Serial1.available()) {
    getdata = Serial1.read();
    Serial.print(getdata, DEC);
  }

    if (getdata == 67)
        {
          digitalWrite(RelayPin, turnoff) ;
        }

    if (getdata == 79)
        {
          digitalWrite(RelayPin, turnon) ;
        }

  LDateTime.getTime(&t);
  String analogdata = String(analogRead(SensorPin), DEC);
  String tramformdata = String(analogRead(SensorPin), DEC);
  String datedata = String(t.year, DEC)+"/"+String(t.mon, DEC)+"/"+String(t.day, DEC);
  String timedata = String(t.hour, DEC)+":"+String(t.min, DEC)+":"+String(t.sec, DEC);
  Serial.print("Data is :(");
  Serial.print(analogdata);
  Serial.print(")\n");
  
  // Print Update Response to Serial Monitor
  while (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }
  
    updateThingSpeak("field1="+datedata+"&field2="+timedata+"&field3="+analogdata+"&field4="+tramformdata);
    Serial.println("Update thingspeak is ok");
  delay(10000) ;
}
//===============
void updateThingSpeak(String tsData)
{
  if (client.connect(SITE_URL, 80))
  { 
    Serial.println("Connected to ThingSpeak Now...");
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





