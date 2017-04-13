#include <LWiFi.h>
#include <LWiFiClient.h>
#define SITE_URL "www.mediatek.com"
#define WIFI_AP "linkitone" //請輸入所要連的AP名稱
#define WIFI_PWD "" // 請輸入AP 密碼
LWiFiClient c;
IPAddress ip(192, 168, 30, 200);
IPAddress dnServer(168, 95, 1, 1);
// the router's gateway address:
IPAddress gateway(192, 168, 30, 254);
// the subnet:
IPAddress subnet(255, 255, 255, 0);
// ThingSpeak Settings
byte server[]  = { 184, 106, 153, 149 }; // IP Address for the ThingSpeak API
String writeAPIKey = "UR42ZTYTE4PBQ8Y2 ";    // Write API Key for a ThingSpeak Channel
const int updateInterval = 30000;        // Time interval in milliseconds to update ThingSpeak   

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
 if(!c.connect(SITE_URL, 80))
 {
 Serial.println("FAIL!");
 return;
 }
 Serial.println("ok");
 Serial.println("send HTTP GET request");
 c.println("GET / HTTP/1.1");
 c.println("Host: " SITE_URL);
 c.println("Connection: close");
 c.println();
}

void loop() {
 int v;
 while(c.available())
 {
 v = c.read();
 if(v < 0)
 break;
 Serial.print((char)v);
 }
 delay(100);
}
