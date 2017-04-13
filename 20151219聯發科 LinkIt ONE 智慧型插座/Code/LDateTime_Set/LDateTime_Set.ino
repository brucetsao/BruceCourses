#include <LDateTime.h>
#include "ldatetimevar.h"


datetimeInfo t;
unsigned int rtc;
LdateInfo nowdate ;
LtimeInfo nowtime ;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  nowdate.year = 2015 ;
  nowdate.month = 12 ;
  nowdate.day = 1 ;
  nowtime.hour = 12 ;
  nowtime.minute = 10 ;
  nowtime.second = 20 ;
  
  LDateTime.getTime(&t);
  LDateTime.getRtc(&rtc);
  // now show date time
  ShowDateTime(t) ;
//=========now set new time1
  setLdate(nowdate) ;
  setLtime(nowtime) ;
  
  // now show date time
  LDateTime.getTime(&t);
  ShowDateTime(t) ;
  
  
  delay(1000);
}

void ShowDateTime(datetimeInfo tt)
{
   Serial.print("year:") ;
  Serial.print(tt.year) ;
  Serial.print("\n") ;
  Serial.print("month:") ;
  Serial.print(tt.mon) ;
  Serial.print("\n") ;
  Serial.print("day:") ;
  Serial.print(tt.day) ;
  Serial.print("\n") ;
  Serial.print("hour:") ;
  Serial.print(tt.hour) ;
  Serial.print("\n") ;
  Serial.print("minute:") ;
  Serial.print(tt.min) ;
  Serial.print("\n") ;
  Serial.print("second:") ;
  Serial.print(tt.sec) ;
  Serial.print("\n") ;
 
}
void setLdate(LdateInfo v)
{
 datetimeInfo p;
   LDateTime.getTime(&p);
    p.year = v.year ;
    p.mon = v.month ;
    p.day = v.day ;
     LDateTime.setTime(&p);
  
}

void setLtime(LtimeInfo v)
{
 datetimeInfo p;
   LDateTime.getTime(&p);
    p.hour = v.hour ;
    p.min = v.minute ;
    p.sec = v.second ;
     LDateTime.setTime(&p);
  
}
