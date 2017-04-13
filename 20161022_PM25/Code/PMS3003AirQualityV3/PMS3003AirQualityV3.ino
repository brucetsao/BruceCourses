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

#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX

#define pmsDataLen 32
uint8_t buf[pmsDataLen];
int idx = 0;
int pm25 = 0;
uint16_t PM01Value=0;          //define PM1.0 value of the air detector module
uint16_t PM2_5Value=0;         //define PM2.5 value of the air detector module
uint16_t PM10Value=0;         //define PM10 value of the air detector module

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600); // PMS 3003 UART has baud rate 9600
}

void loop() { // run over and over
  idx = 0;
  memset(buf, 0, pmsDataLen);

  while (mySerial.available()) {
    buf[idx++] = mySerial.read();
  }
/*
  // check if data header is correct
  if (buf[0] == 0x42 && buf[1] == 0x4d) {
    pm25 = ( buf[12] << 8 ) | buf[13]; 
    Serial.print("pm2.5: ");
    Serial.print(pm25);
    Serial.println(" ug/m3");
  }
*/
  int checkSum=checkValue(buf,pmsDataLen);
  //if(pmsDataLen&&checkSum)
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


