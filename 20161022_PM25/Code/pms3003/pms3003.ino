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
#include "String.h"

#include <Wire.h>  // Arduino IDE 內建
// LCD I2C Library，從這裡可以下載：
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#include <PowerManagement.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX

#define pmsDataLen 24
unsigned char buf[pmsDataLen];
int idx = 0;
int pm10 = 0;
int pm25 = 0;
int pm100 = 0;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // 設定 LCD I2C 位址
bool hasPm25Value = false;
int ErrorCount = 0 ;
#define ErrorRebootCount 5

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600); // PMS 3003 UART has baud rate 9600
  lcd.begin(20, 4);      // 初始化 LCD，一行 20 的字元，共 4 行，預設開啟背光
  lcd.backlight(); // 開啟背光
  LCDClear() ;
  //  while(!Serial) ;


}

void loop()
{ // run over and over

  retrievePM25Value() ;

  if (hasPm25Value )
    ShowPM()  ;
    
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

