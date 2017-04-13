#include <WiFi.h>
int status = WL_IDLE_STATUS;
void setup() {
  // put your setup code here, to run once:
       Serial.begin(9600) ;
     delay(3000);
     
      Serial.println("Start") ;
    int status = WiFi.status() ;
     if (status)
      {
          Serial.println("Yes") ;
        
      }
      else
      {
          Serial.println("No") ;
      }
}

void loop() {
  // put your main code here, to run repeatedly:

}
