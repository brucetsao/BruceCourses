#include <Wire.h>  
#include "RTClib.h"
RTC_DS1307 RTC;
int mYear = 2016 ;
int mMonth = 1 ;
int mDay = 1 ;
int mHour = 12 ;
int mMinute = 12 ;
int mSecond = 12 ;
void setup() {

  Serial.begin(9600);
     initRTC() ;
    RTC.adjust(DateTime(mYear,mMonth,mDay,mHour,mMinute,mSecond));
    
}

void loop() {
    Serial.print("Now Data and Time is :") ;
    Serial.print(ShowDateTime()) ;
    Serial.print("\n") ;
    delay(1000) ;
}


void initRTC()
{
     Wire.begin();
    RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
   
  }
}
String ShowDateTime()
{

    return StrDate() + "  " +StrTime() ;
}

String  StrDate() {
  String ttt ;
//nowT  = now; 
DateTime now = RTC.now(); 
 ttt = print4digits(now.year()) + "-" + print2digits(now.month()) + "-" + print2digits(now.day()) ;
 //ttt = print4digits(NDPyear) + "/" + print2digits(NDPmonth) + "/" + print2digits(NDPday) ;
  return ttt ;
}

String  StringDate(int yyy,int mmm,int ddd) {
  String ttt ;
//nowT  = now; 
 ttt = print4digits(yyy) + "-" + print2digits(mmm) + "-" + print2digits(ddd) ;
  return ttt ;
}


String  StrTime() {
  String ttt ;
 // nowT  = RTC.now(); 
 DateTime now = RTC.now(); 
  ttt = print2digits(now.hour()) + ":" + print2digits(now.minute()) + ":" + print2digits(now.second()) ;
  //  ttt = print2digits(NDPhour) + ":" + print2digits(NDPminute) + ":" + print2digits(NDPsecond) ;
return ttt ;
}

String  StringTime(int hhh,int mmm,int sss) {
  String ttt ;
  ttt = print2digits(hhh) + ":" + print2digits(mmm) + ":" + print2digits(sss) ;
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



