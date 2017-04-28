#include "Pinset.h"
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?


// How many NeoPixels are attached to the Arduino?

#include <String.h>
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSPIN, NEO_GRB + NEO_KHZ800);

byte RedValue = 0, GreenValue = 0, BlueValue = 0;
String ReadStr = "      " ;

void setup() {
  // put your setup code here, to run once:

    randomSeed(millis());
  Serial.begin(9600) ;
  Serial.println("Program Start Here") ;
  pixels.begin(); // This initializes the NeoPixel library.
    ChangeBulbColor(RedValue,GreenValue,BlueValue) ;
}

int delayval = 500; // delay for half a second

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() >0)
  {
    ReadStr = Serial.readStringUntil(0x23) ;    // read char @
     //  Serial.read() ;
      Serial.print("ReadString is :(") ;
       Serial.print(ReadStr) ;
       Serial.print(")\n") ;
        if (DecodeString(ReadStr,&RedValue,&GreenValue,&BlueValue) )
           {
            Serial.println("Change RGB Led Color") ;
            ChangeBulbColor(RedValue,GreenValue,BlueValue) ;
           }
  }

  
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
                    Serial.print("check sgtring:(") ;
                    Serial.print(INPStr) ;
                         Serial.print(")\n") ;

      int i = 0 ;
      int strsize = INPStr.length();
      for(i = 0 ; i <strsize ;i++)
          {
                    Serial.print(i) ;
                    Serial.print(":(") ;
                         Serial.print(INPStr.substring(i,i+1)) ;
                    Serial.print(")\n") ;

              if (INPStr.substring(i,i+1) == "@")
                  {
                    Serial.print("find @ at :(") ;
                    Serial.print(i) ;
                         Serial.print("/") ;
                              Serial.print(strsize-i-1) ;
                         Serial.print("/") ;
                              Serial.print(INPStr.substring(i+1,strsize)) ;
                    Serial.print(")\n") ;
                      *r = byte(INPStr.substring(i+1,i+1+3).toInt()) ;
                      *g = byte(INPStr.substring(i+1+3,i+1+3+3).toInt() ) ;
                      *b = byte(INPStr.substring(i+1+3+3,i+1+3+3+3).toInt() ) ;
                       Serial.print("convert into :(") ;
                        Serial.print(*r) ;
                         Serial.print("/") ;
                        Serial.print(*g) ;
                         Serial.print("/") ;
                        Serial.print(*b) ;
                         Serial.print(")\n") ;
                            
                      return true ;
                  }
          }
    return false ;

}

