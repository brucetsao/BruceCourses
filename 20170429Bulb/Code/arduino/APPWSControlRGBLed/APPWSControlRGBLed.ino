
#include "Pinset.h"
#include <SoftwareSerial.h>
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?


// How many NeoPixels are attached to the Arduino?


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSPIN, NEO_GRB + NEO_KHZ800);

byte RedValue = 0, GreenValue = 0, BlueValue = 0;
String ReadStr = "      " ;
SoftwareSerial mySerial(RxPin, TxPin); // RX, TX

void setup() {
  // put your setup code here, to run once:
     Serial.begin(9600) ;
    DebugMsgln("Program Start Here") ;
      mySerial.begin(9600); //
      pixels.begin(); // This initializes the NeoPixel library.
      DebugMsgln("init LED") ;
    ChangeBulbColor(RedValue,GreenValue,BlueValue) ;
      DebugMsgln("Turn off LED") ;
      if (TestLed ==  1)
          {
               CheckLed() ;
                 DebugMsgln("Check LED") ;
                  ChangeBulbColor(RedValue,GreenValue,BlueValue) ;
                  DebugMsgln("Turn off LED") ;
          }
 

      DebugMsgln("Clear Bluetooth Buffer") ;
      ClearBluetoothBuffer() ;
      
    delay(initDelayTime) ;   //wait 2 seconds
     DebugMsgln("Waiting for Bluetooth Connection") ;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (mySerial.available()>0)
  {
      DebugMsgln("Data Comming") ;
    ReadStr = mySerial.readStringUntil(0x23) ;    // read char @
     //  Serial.read() ;
      DebugMsg("ReadString is :(") ;
       DebugMsg(ReadStr) ;
       DebugMsg(")\n") ;
        if (DecodeString(ReadStr,&RedValue,&GreenValue,&BlueValue) )
           {
            DebugMsgln("Change RGB Led Color") ;
            ChangeBulbColor(RedValue,GreenValue,BlueValue) ;
              mySerial.print("OK") ;
           }
     
  }
 //  delay(CommandDelay) ;
  // Serial.print(".") ;
}

void ChangeBulbColor(int r,int g,int b)
{
      // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
   for(int i=0;i<NUMPIXELS;i++)
   {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(r,g,b)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
       // delay(delayval); // Delay for a period of time (in milliseconds).
  }
}

boolean DecodeString(String INPStr, byte *r, byte *g , byte *b) 
{
                    DebugMsg("check sgtring:(") ;
                    DebugMsg(INPStr) ;
                         DebugMsg(")\n") ;

      int i = 0 ;
      int strsize = INPStr.length();
      for(i = 0 ; i <strsize ;i++)
          {
                    DebugMsg(String(i,DEC)) ;
                    DebugMsg(":(") ;
                         DebugMsg(INPStr.substring(i,i+1)) ;
                    DebugMsg(")\n") ;

              if (INPStr.substring(i,i+1) == "@")
                  {
                    DebugMsg("find @ at :(") ;
                   DebugMsg(String(i,DEC)) ;
                         DebugMsg("/") ;
                              DebugMsg(String(strsize-i-1,DEC)) ;
                         DebugMsg("/") ;
                              DebugMsg(INPStr.substring(i+1,strsize)) ;
                    DebugMsg(")\n") ;
                      *r = byte(INPStr.substring(i+1,i+1+3).toInt()) ;
                      *g = byte(INPStr.substring(i+1+3,i+1+3+3).toInt() ) ;
                      *b = byte(INPStr.substring(i+1+3+3,i+1+3+3+3).toInt() ) ;
                       DebugMsg("convert into :(") ;
                        DebugMsg(String(*r,DEC)) ;
                         DebugMsg("/") ;
                        DebugMsg(String(*g,DEC)) ;
                         DebugMsg("/") ;
                        DebugMsg(String(*b,DEC)) ;
                         DebugMsg(")\n") ;
                            
                      return true ;
                  }
          }
    return false ;

}

void CheckLed()
{
     for(int i = 0 ; i <16; i++)
        {
              ChangeBulbColor(CheckColor[i][0],CheckColor[i][1],CheckColor[i][2]) ;
              delay(CheckColorDelayTime) ;
        }
}

void ClearBluetoothBuffer()
{
    
    while (mySerial.available() >0)
        {
          DebugMsg(String(mySerial.read()))  ;
        }
    DebugMsg("END \n") ;
}

void DebugMsg(String msg)
{
    if (_Debug != 0)
        {
          Serial.print(msg) ;
        }
   
}
void DebugMsgln(String msg)
{
    if (_Debug != 0)
        {
          Serial.println(msg) ;
        }
   
}
