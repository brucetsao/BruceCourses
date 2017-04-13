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
 * RX is digital pin 0 (connect to TX of PMS 3003)
 * TX is digital pin 1 (connect to RX of PMS 3003)

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
#define DHTSensorPin 8
#include "DHT.h"
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#include <Wire.h>  // Arduino IDE 內建
// LCD I2C Library，從這裡可以下載：
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

uint8_t MacData[6];

SoftwareSerial mySerial(0, 1); // RX, TX
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // 設定 LCD I2C 位址
DHT dht(DHTSensorPin, DHTTYPE);


void setup() {
  initPins() ;
  Serial.begin(9600);
   dht.begin();
  mySerial.begin(9600); // PMS 3003 UART has baud rate 9600
  lcd.begin(20, 4);      // 初始化 LCD，一行 20 的字元，共 4 行，預設開啟背光
       lcd.backlight(); // 開啟背光
     //  while(!Serial) ;

}

void loop() { // run over and over
    ShowHumidity() ;
  delay(2000);  
}
void ShowHumidity()
{
    float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
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
    lcd.setCursor(11, 3); // 設定游標位置在第一行行首
      lcd.print((int)h);
     lcd.print("% ");
     lcd.print((int)t);
 
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


void initPins()
{
pinMode(DHTSensorPin,INPUT) ;

}


