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
 #include <String.h> 
#include <Wire.h>  // Arduino IDE 內建
// LCD I2C Library，從這裡可以下載：
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#include <LiquidCrystal_I2C.h>

#include <SoftwareSerial.h>

#include <WiFi.h>

uint8_t MacData[6];

SoftwareSerial mySerial(0, 1); // RX, TX

#define pmsDataLen 32
uint8_t buf[pmsDataLen];
int idx = 0;
int pm25 = 0;
uint16_t PM01Value=0;          //define PM1.0 value of the air detector module
uint16_t PM2_5Value=0;         //define PM2.5 value of the air detector module
uint16_t PM10Value=0;         //define PM10 value of the air detector module

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // 設定 LCD I2C 位址
String MacAddress ;
void setup() {
  Serial.begin(9600);

  mySerial.begin(9600); // PMS 3003 UART has baud rate 9600
  lcd.begin(20, 4);      // 初始化 LCD，一行 16 的字元，共 2 行，預設開啟背光
       lcd.backlight(); // 開啟背光
WiFi.status();
MacAddress = GetWifiMac() ;

  ShowMac() ;
}

void loop() { // run over and over
  idx = 0;
  memset(buf, 0, pmsDataLen);

  while (mySerial.available()) {
    buf[idx++] = mySerial.read();
  }

  // check if data header is correct
  if (buf[0] == 0x42 && buf[1] == 0x4d) {
    pm25 = ( buf[12] << 8 ) | buf[13]; 
    Serial.print("pm2.5: ");
    Serial.print(pm25);
    Serial.println(" ug/m3");
    ShowPM(pm25) ;
  }
/*
  int checkSum=checkValue(buf,pmsDataLen);
  if(pmsDataLen&&checkSum)
  {
    PM01Value=transmitPM01(buf);
    PM2_5Value=transmitPM2_5(buf);
    PM10Value=transmitPM10(buf);
  }
    static unsigned long OledTimer=millis();
  if (millis() - OledTimer >=1000)
  {
    OledTimer=millis();
      Serial.print("PM1.0: ");
      Serial.print(PM01Value);
      Serial.println("  ug/m3");
      Serial.print("PM2.5: ");
      Serial.print(PM2_5Value);
      Serial.println("  ug/m3");
      Serial.print("PM10:  "); //send PM1.0 data to bluetooth
      Serial.print(PM10Value);
      Serial.println("ug/m3");
  }
*/

}

uint8_t checkValue(uint8_t *thebuf, uint8_t leng)
{  
  uint8_t receiveflag=1;
  uint16_t receiveSum=0;
  uint8_t i=0;

  for(i=0;i<leng;i++)
  {
  receiveSum=receiveSum+thebuf[i];
  }
  
  if(receiveSum==((thebuf[leng-2]<<8)+thebuf[leng-1]+thebuf[leng-2]+thebuf[leng-1]))  //check the serial data 
  {
    receiveSum=0;
  receiveflag=1;
//  Serial.print(receiveflag);
  return receiveflag;
  }
}
//transmit PM Value to PC
uint16_t transmitPM01(uint8_t *thebuf)
{

  uint16_t PM01Val;

  PM01Val=((thebuf[4]<<8) + thebuf[5]); //count PM1.0 value of the air detector module
  return PM01Val;
}

//transmit PM Value to PC
uint16_t transmitPM2_5(uint8_t *thebuf)
{
  uint16_t PM2_5Val;

    PM2_5Val=((thebuf[6]<<8) + thebuf[7]);//count PM2.5 value of the air detector module

  return PM2_5Val;
  }

//transmit PM Value to PC
uint16_t transmitPM10(uint8_t *thebuf)
{
  
  uint16_t PM10Val;

    PM10Val=((thebuf[8]<<8) + thebuf[9]); //count PM10 value of the air detector module
  
  return PM10Val;
  }

void ShowMac()
{
     lcd.setCursor(0, 0); // 設定游標位置在第一行行首
     lcd.print("MAC:");
     lcd.print(MacAddress);

}

void ShowPM(int pp25)
{
    lcd.setCursor(0, 1); // 設定游標位置在第一行行首
     lcd.print("PM2.5:");
     lcd.print(pp25);

}

String GetWifiMac()
{
   String tt ;
    String t1,t2,t3,t4,t5,t6 ;
  WiFi.macAddress(MacData);
  
  Serial.print("Mac:");
   t1 = print2HEX((int)MacData[0]);
   t2 = print2HEX((int)MacData[1]);
   t3 = print2HEX((int)MacData[2]);
   t4 = print2HEX((int)MacData[3]);
   t5 = print2HEX((int)MacData[4]);
   t6 = print2HEX((int)MacData[5]);
 tt = (t1+t2+t3+t4+t5+t6) ;
Serial.print(tt);
Serial.print("\n");
  
  return tt ;
}

String  print2HEX(int number) {
  String ttt ;
  if (number >= 0 && number < 16)
  {
    ttt = String("0") + String(number,HEX);
  }
  else
  {
      ttt = String(number,HEX);
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




