/*
  This example demonstrate how to read pm2.5 value on PMS 3003 air condition sensor

  PMS 3003 pin map is as follow:
    PIN1  :VCC, connect to 5V
    PIN2  :GND
    PIN3  :SET, 0:Standby mode, 1:operating mode
    PIN4  :RXD :Serial RX
    PIN5  :TXD :Serial TX
    PIN6  :RESET
    PIN7  :NC
    PIN8  :NC

  In this example, we only use Serial to get PM 2.5 value.

  The circuit:
   RX is digital pin 0 (connect to TX of PMS 3003)
   TX is digital pin 1 (connect to RX of PMS 3003)

*/
/*
  This example demonstrate how to upload sensor data to MQTT server of LASS.
  It include features:
     (1) Connect to WiFi
     (2) Retrieve NTP time with WiFiUDP
     (3) Get PM 2.5 value from PMS3003 air condition sensor with UART
     (4) Connect to MQTT server and try reconnect when disconnect

  You can find more information at this site:

     https://lass.hackpad.com/LASS-README-DtZ5T6DXLbu

*/

//  http://nrl.iis.sinica.edu.tw/LASS/show.php?device_id=FT1_074B3
#include <math.h>
#include <PowerManagement.h>

#define turnon HIGH
#define turnoff LOW
#define DHTSensorPin 7
#define ParticleReset 8
#define ParticleSensorLed 9
#define InternetLed 10
#define AccessLed 11

#include "String.h"
#include "PMType.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
// THIS INLCUDE LIB FOR dhx sensor
#include "DHT.h"
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#include <Wire.h>  // Arduino IDE 內建
// LCD I2C Library，從這裡可以下載：
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads

#include "RTClib.h"
RTC_DS1307 RTC;
DateTime nowT  ;

#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

uint8_t MacData[6];

SoftwareSerial mySerial(0, 1); // RX, TX
//char ssid[] = "PM25";      // your network SSID (name)
//char pass[] = "12345678";     // your network password
char ssid[] = "TSAO_1F";      // your network SSID (name)
char pass[] = "TSAO1234";     // your network password
//  使用者改上面的AP名字與AP 密碼就可以換成您自己的


int keyIndex = 0;               // your network key Index number (needed only for WEP)

const char gps_lat[] = "23.954710";
const char gps_lon[] = "120.574482";
const char gps_alt[] = "30";
//  使用者改上面的gps_lat[] & gps_lon[] 經緯度內容就可以換成您自己的位置

char server[] = "gpssensor.ddns.net"; // the MQTT server of LASS

#define MAX_CLIENT_ID_LEN 10
#define MAX_TOPIC_LEN     50
char clientId[MAX_CLIENT_ID_LEN];
char outTopic[MAX_TOPIC_LEN];

WiFiClient wifiClient;
PubSubClient client(wifiClient);
IPAddress  Meip , Megateway , Mesubnet ;
String MacAddress ;
int status = WL_IDLE_STATUS;
boolean ParticleSensorStatus = true ;
WiFiUDP Udp;

const char ntpServer[] = "pool.ntp.org";
const long timeZoneOffset = 28800L;
//const long timeZoneOffset = 0L;
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
const byte nptSendPacket[ NTP_PACKET_SIZE] = {
    0xE3, 0x00, 0x06, 0xEC, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x31, 0x4E, 0x31, 0x34,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
byte ntpRecvBuffer[ NTP_PACKET_SIZE ];

#define LEAP_YEAR(Y)     ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )
static  const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // API starts months from 1, this array starts from 0
uint32_t epochSystem = 0; // timestamp of system boot up


#define pmsDataLen 24
unsigned char buf[pmsDataLen];
int idx = 0;
int pm10 = 0;
int pm25 = 0;
int pm100 = 0;
int NDPyear, NDPmonth, NDPday, NDPhour, NDPminute, NDPsecond;
int HumidityData = 0 ;
int TemperatureData = 0 ;
unsigned long epoch  ;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // 設定 LCD I2C 位址
DHT dht(DHTSensorPin, DHTTYPE);
bool hasPm25Value = false;
int ErrorCount = 0 ;
#define ErrorRebootCount 5

void setup()
{
  initPins() ;
  Serial.begin(9600);
  dht.begin();
  mySerial.begin(9600); // PMS 3003 UART has baud rate 9600
  lcd.begin(20, 4);      // 初始化 LCD，一行 20 的字元，共 4 行，預設開啟背光
  lcd.backlight(); // 開啟背光
  LCDClear() ;
  //  while(!Serial) ;


  MacAddress = GetWifiMac() ;
  ShowMac() ;
  if  (initializeWiFi())
  {

    initRTC() ;
    ShowDateTime() ;
    showLed() ;
    ShowInternetStatus() ;
    initializeMQTT();
        delay(10000);
  }
  else
  {
    Serial.println("initializeWiFi Fail...") ;
    Serial.println("Reboot.....");
    delay(5000) ;
    SystemReboot() ;
  }
}

void loop()
{ // run over and over

  ShowError() ;
  retrievePM25Value() ;

  if (hasPm25Value )
    ShowPM()  ;
    
  showLed() ;
  ShowDateTime() ;
  ShowHumidity() ;
  if (!client.connected())
  {
    reconnectMQTT();
      showLed() ;
  }
  client.loop();
  // Serial.println(millis()) ;

  delay(10000); // delay 1 minute for next measurement
  //
   status = WiFi.status() ; 
  if (status != WL_CONNECTED)
       {
           ErrorCount ++ ;
           ReConnectInternet();
       }
       else
       {
        
         ErrorCount = 0 ;
       }
  if (ErrorCount > ErrorRebootCount)
  {
      Serial.println("Clear System Buffer") ;
       ClearSerialBuffer() ;
       Serial.println("ReBooting....") ;
        SystemReboot() ;
  }
}

void ShowMac()
{
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print("MAC:");
  lcd.print(MacAddress);

}

void ShowInternetStatus()
{
  lcd.setCursor(0, 1); // 設定游標位置
  if (WiFi.status())
  {
    Meip = WiFi.localIP();
    lcd.print("@:");
    lcd.print(Meip);
    digitalWrite(InternetLed, turnon) ;
  }
  else
  {
    lcd.print("DisConnected:");
    digitalWrite(InternetLed, turnoff) ;
  }

}
void ShowHumidity()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  HumidityData = (int)h ;
  TemperatureData = (int)t ;
  Serial.print("Humidity :") ;
  Serial.print(h) ;
  Serial.print("%  /") ;
  Serial.print(t) ;
  Serial.print("C  \n") ;
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  lcd.setCursor(14, 3); // 設定游標位置在第一行行首
  lcd.print((int)h);
  lcd.print("%");
  lcd.print((int)t);
  lcd.print("C");

}

void ShowPM()
{

  lcd.setCursor(0, 3); // 設定游標位置在第一行行首
  lcd.print("             ");
  lcd.setCursor(0, 3); // 設定游標位置在第一行行首
  lcd.print("S:");
  lcd.print(pm25);
  lcd.print("/");
  lcd.print(pm10);
  lcd.print("/");
  lcd.print(pm100);

}



void ShowDateTime()
{
  Serial.print(StrDate()) ;
  Serial.print("----") ;
  Serial.print(StrTime()) ;
  Serial.print("\n") ;
  //  getCurrentTime(epoch, &NDPyear, &NDPmonth, &NDPday, &NDPhour, &NDPminute, &NDPsecond);
  lcd.setCursor(0, 2); // 設定游標位置在第一行行首
  lcd.print(StrDate());
  lcd.setCursor(11, 2); // 設定游標位置在第一行行首
  lcd.print(StrTime());
  //  lcd.print();

}

String  StrDate() {
  String ttt ;
  //nowT  = now;
  nowT = RTC.now();
  // ttt = print4digits(nowT.year()) + "-" + print2digits(nowT.month()) + "-" + print2digits(nowT.day()) ;
  ttt = StringDate(nowT.year(), nowT.month(), nowT.day()) ;
  //ttt = print4digits(NDPyear) + "/" + print2digits(NDPmonth) + "/" + print2digits(NDPday) ;
  return ttt ;
}

String  StringDate(int yyy, int mmm, int ddd) {
  String ttt ;
  //nowT  = now;
  ttt = print4digits(yyy) + "-" + print2digits(mmm) + "-" + print2digits(ddd) ;
  return ttt ;
}


String  StrTime() {
  String ttt ;
  // nowT  = RTC.now();
  nowT = RTC.now();
  //  ttt = print2digits(nowT.hour()) + ":" + print2digits(nowT.minute()) + ":" + print2digits(nowT.second()) ;
  ttt = StringTime(nowT.hour(), nowT.minute(), nowT.second()) ;
  //  ttt = print2digits(NDPhour) + ":" + print2digits(NDPminute) + ":" + print2digits(NDPsecond) ;
  return ttt ;
}

String  StringTime(int hhh, int mmm, int sss) {
  String ttt ;
  ttt = print2digits(hhh) + ":" + print2digits(mmm) + ":" + print2digits(sss) ;
  return ttt ;
}


String GetWifiMac()
{
  String tt ;
  String t1, t2, t3, t4, t5, t6 ;
  WiFi.status();    //this method must be used for get MAC
  WiFi.macAddress(MacData);

  Serial.print("Mac:");
  Serial.print(MacData[0], HEX) ;
  Serial.print("/");
  Serial.print(MacData[1], HEX) ;
  Serial.print("/");
  Serial.print(MacData[2], HEX) ;
  Serial.print("/");
  Serial.print(MacData[3], HEX) ;
  Serial.print("/");
  Serial.print(MacData[4], HEX) ;
  Serial.print("/");
  Serial.print(MacData[5], HEX) ;
  Serial.print("~");

  t1 = print2HEX((int)MacData[0]);
  t2 = print2HEX((int)MacData[1]);
  t3 = print2HEX((int)MacData[2]);
  t4 = print2HEX((int)MacData[3]);
  t5 = print2HEX((int)MacData[4]);
  t6 = print2HEX((int)MacData[5]);
  tt = (t1 + t2 + t3 + t4 + t5 + t6) ;
  tt.toUpperCase() ;
  Serial.print(tt);
  Serial.print("\n");

  return tt  ;
}
String  print2HEX(int number) {
  String ttt ;
  if (number >= 0 && number < 16)
  {
    ttt = String("0") + String(number, HEX);
  }
  else
  {
    ttt = String(number, HEX);
  }
  return ttt ;
}
String  print2digits(int number) {
  String ttt ;
  if (number >= 0 && number < 10)
  {
    ttt = String("0") + String(number);
  }
  else
  {
    ttt = String(number);
  }
  return ttt ;
}

String  print4digits(int number) {
  String ttt ;
  ttt = String(number);
  return ttt ;
}



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// send an NTP request to the time server at the given address
void retrieveNtpTime() {
  Serial.println("Send NTP packet");

  Udp.beginPacket(ntpServer, 123); //NTP requests are to port 123
  Udp.write(nptSendPacket, NTP_PACKET_SIZE);
  Udp.endPacket();

  if (Udp.parsePacket()) {
    Serial.println("NTP packet received");
    Udp.read(ntpRecvBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    unsigned long highWord = word(ntpRecvBuffer[40], ntpRecvBuffer[41]);
    unsigned long lowWord = word(ntpRecvBuffer[42], ntpRecvBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    //     epoch = secsSince1900 - seventyYears + timeZoneOffset ;
    epoch = secsSince1900 - seventyYears ;

    epochSystem = epoch - millis() / 1000;
  }
}


void getCurrentTime(unsigned long epoch, int *year, int *month, int *day, int *hour, int *minute, int *second) {
  int tempDay = 0;

  *hour = (epoch  % 86400L) / 3600;
  *minute = (epoch  % 3600) / 60;
  *second = epoch % 60;

  *year = 1970;
  *month = 0;
  *day = epoch / 86400;

  for (*year = 1970; ; (*year)++) {
    if (tempDay + (LEAP_YEAR(*year) ? 366 : 365) > *day) {
      break;
    } else {
      tempDay += (LEAP_YEAR(*year) ? 366 : 365);
    }
  }
  tempDay = *day - tempDay; // the days left in a year
  for ((*month) = 0; (*month) < 12; (*month)++) {
    if ((*month) == 1) {
      if (LEAP_YEAR(*year)) {
        if (tempDay - 29 < 0) {
          break;
        } else {
          tempDay -= 29;
        }
      } else {
        if (tempDay - 28 < 0) {
          break;
        } else {
          tempDay -= 28;
        }
      }
    } else {
      if (tempDay - monthDays[(*month)] < 0) {
        break;
      } else {
        tempDay -= monthDays[(*month)];
      }
    }
  }
  (*month)++;
  *day = tempDay + 2; // one for base 1, one for current day
}

void reconnectMQTT() {
  int connectWifiCount = 0 ;
  // Loop until we're reconnected
  char payload[400];
  nowT = RTC.now();
  digitalWrite(AccessLed, turnon) ;

  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientId))
    {
      Serial.println("connected");

      //      sprintf(payload, "|ver_format=3|fmt_opt=1|app=Pm25Ameba|ver_app=0.0.1|device_id=%s|tick=%d|date=%4d-%02d-%02d|time=%02d:%02d:%02d|device=Ameba|s_d0=%d|gps_lat=%s|gps_lon=%s|gps_fix=1|gps_num=9|gps_alt=2",
      //      sprintf(payload, "|ver_format=3|fmt_opt=0|app=PM25|ver_app=0.7.3|device_id=%s|tick=%d|date=%4d-%02d-%02d|time=%02d:%02d:%02d|device=Ameba|s_d0=%d|s_d1=%d|s_d2=%d|s_h0=%d|s_t0=%d|gps_lat=%s|gps_lon=%s|gps_fix=1|gps_num=12|gps_alt=%s",
      sprintf(payload, "|FAKE_GPS=1|ver_format=3|fmt_opt=0|app=PM25|ver_app=0.0.1|device_id=%s|tick=%d|date=%4d-%02d-%02d|time=%02d:%02d:%02d|device=Ameba|s_d0=%d|s_d1=%d|s_d2=%d|s_h0=%d|s_t0=%d|gps_lat=%s|gps_lon=%s|gps_fix=1|gps_num=12|gps_alt=11",
              clientId,
              millis(),
              (int)nowT.year(), (int)nowT.month(), (int)nowT.day(),
              (int)nowT.hour(), (int)nowT.minute(), (int)nowT.second(),
              pm25, pm100, pm10,
              HumidityData,
              TemperatureData,
              gps_lat, gps_lon, gps_alt
             );

      // Once connected, publish an announcement...
      client.publish(outTopic, payload);
    }
    else
    {
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 5 seconds");
          // Wait 5 seconds before retrying
          delay(1000);
          connectWifiCount ++ ;
    }
    if (connectWifiCount > 3)
    {
      Serial.println("Connect MQTT Error") ;
        break ;
     // ReConnectInternet();
    }
  }
  digitalWrite(AccessLed, turnoff) ;
}


void retrievePM25Value()
{
  hasPm25Value = false;
  int  count = 0;
  int ckecksum = 0 ;
  int exptsum = 0 ;
  memset(buf, 0, pmsDataLen);

  while (( mySerial.available()>0) && (count < pmsDataLen) )
  {
    //    mySerial.readBytes(buf,pmsDataLen);
    buf[count] = mySerial.read();
    /*
      Serial.print("NO(") ;
      Serial.print(count) ;
      Serial.print(") , ") ;
      Serial.print( buf[count],DEC) ;
      Serial.print("\n") ;
    */
    if (buf[0] == 0x42 && count == 0 )
    {
      count = 1 ;
      continue ;

    }
    if (buf[1] == 0x4d && count == 1 )
    {
      count = 2 ;
      continue ;

    }
    if (count >= 2 )
    {
      count++ ;
      if (count >= (pmsDataLen) )
      {
        hasPm25Value = true ;
        break ;
      }
      continue ;
    }

  }


  if (hasPm25Value )
  {
    for (int i = 0 ; i < (pmsDataLen - 2) ; i++)
      ckecksum = ckecksum + buf[i] ;
    exptsum = ((unsigned int)buf[22] << 8 ) + ((unsigned int)buf[23]) ;
    if (ckecksum == exptsum)
    {
      hasPm25Value = true ;
    }
    else
    {
      hasPm25Value = false ;
      Serial.print("\n \n ERROR Check sum");
      Serial.print("\n Sensor Check sum is : ");
      Serial.print(exptsum);
      Serial.print(", \n And Data Check sum is :");
      Serial.print(ckecksum);
      Serial.println("");
      return ;
    }
    pm25 = ( buf[12] << 8 ) | buf[13];
    pm10 = ( buf[10] << 8 ) | buf[11];
    pm100 = ( buf[14] << 8 ) | buf[15];

    Serial.print("\n pm2.5: ");
    Serial.print(pm25);
    Serial.print(" ug/m3\t");
    Serial.print("pm1.0: ");
    Serial.print(pm10);
    Serial.print(" ug/m3\t");
    Serial.print("pm100: ");
    Serial.print(pm100);
    Serial.print(" ug/m3");
    Serial.println("");
  }


}

boolean initializeWiFi() {
  int initWifiErrorCount = 0 ;
//  status = WiFi.status() ;
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
  //  使用者改上面的WiFi.begin(ssid, pass); 就是連到AP使用密碼
    //   status = WiFi.begin(ssid);
  //  使用者改上面的WiFi.begin(ssid) 就是連到AP不用密碼
    if (initWifiErrorCount > 3)
          return false ;
    // wait 10 seconds for connection:
    
    initWifiErrorCount++ ;
    Serial.print("Try to Connectting AP:(") ;
    Serial.print(initWifiErrorCount) ;
    Serial.print(")\n") ;
    //delay(5000);
  }

  // local port to listen for UDP packets
  Udp.begin(2390);
  return true ;
}

void initializeMQTT() {
  digitalWrite(AccessLed, turnon) ;
  // byte mac[6];
  // WiFi.macAddress(MacData);
  memset(clientId, 0, MAX_CLIENT_ID_LEN);
  sprintf(clientId, "FT1_0%02X%02X", MacData[4], MacData[5]);
  //  sprintf(outTopic, "LASS/Test/PM25/%s", clientId);
  sprintf(outTopic, "LASS/Test/PM25");

  Serial.print("MQTT client id:");
  Serial.println(clientId);
  Serial.print("MQTT topic:");
  Serial.println(outTopic);

  client.setServer(server, 1883);
  client.setCallback(callback);

  digitalWrite(AccessLed, turnoff) ;
}

void printWifiData()
{
  // print your WiFi shield's IP address:
  Meip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(Meip);


  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

  // print your subnet mask:
  Mesubnet = WiFi.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(Mesubnet);

  // print your gateway address:
  Megateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(Megateway);
}

void showLed()
{
  if (ParticleSensorStatus)
  {
    digitalWrite(ParticleSensorLed, turnon) ;
  }
  else
  {
    digitalWrite(ParticleSensorLed, turnoff) ;
  }
  if (status == WL_CONNECTED)
  {
    digitalWrite(InternetLed, turnon) ;
  }
  else
  {
    digitalWrite(InternetLed, turnoff) ;
  }


}


void initRTC()
{
  Wire.begin();
  RTC.begin();
  if (WiFi.status())
    SetRTCFromNtpTime() ;
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //    RTC.adjust(DateTime(__DATE__, __TIME__));

  }
}
void initPins()
{

  pinMode(DHTSensorPin, INPUT) ;
  pinMode(ParticleReset, OUTPUT) ;
  pinMode(ParticleSensorLed, OUTPUT) ;
  pinMode(InternetLed, OUTPUT) ;
  pinMode(AccessLed, OUTPUT) ;
  digitalWrite(ParticleReset, turnoff) ;
  digitalWrite(ParticleSensorLed, turnon) ;
  digitalWrite(InternetLed, turnoff) ;
  digitalWrite(AccessLed, turnon) ;
}
void SetRTCFromNtpTime()
{
  retrieveNtpTime();
  //DateTime ttt;
  //    getCurrentTime(epoch+timeZoneOffset, &NDPyear, &NDPmonth, &NDPday, &NDPhour, &NDPminute, &NDPsecond);
  getCurrentTime(epoch, &NDPyear, &NDPmonth, &NDPday, &NDPhour, &NDPminute, &NDPsecond);
  //ttt->year = NDPyear ;
  Serial.print("NDP Date is :");
  Serial.print(StringDate(NDPyear, NDPmonth, NDPday));
  Serial.print("and ");
  Serial.print("NDP Time is :");
  Serial.print(StringTime(NDPhour, NDPminute, NDPsecond));
  Serial.print("\n");

  //        RTC.adjust(DateTime(epoch+timeZoneOffset));
  RTC.adjust(DateTime(epoch));
  nowT = RTC.now();
}

void ResetPMSensor()
{
  digitalWrite(ParticleReset, turnoff) ;
  delay(100) ;
  digitalWrite(ParticleReset, turnon) ;
  Serial.println("Reset PM Sensor") ;
}

boolean ReConnectInternet()
{
  LCDClear() ;
  MacAddress = GetWifiMac() ;
  ShowMac() ;
  initializeWiFi();

  initRTC() ;
  ShowDateTime() ;
  showLed() ;
  ShowInternetStatus() ;

  delay(1000);
  initializeMQTT();

}


void SystemReboot()
{
  lcd.clear() ;
  lcd.print("Now Rebooting.....") ;
  delay(3000) ;
  PowerManagement.softReset() ;
}

void ShowError()
{
  lcd.setCursor(18, 0); // 設定游標位置在第一行行首
  lcd.print(ErrorCount) ;
}

void LCDClear()
{
   lcd.clear() ;
}

void ClearSerialBuffer()
{
while ( mySerial.available() >0)
    mySerial.read() ;
}

