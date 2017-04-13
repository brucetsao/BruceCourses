//  Author Bruce 
void setup(){

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("program start here");
  Serial.println("BlueTooth Test...");

}
  
void loop(){
  char val ;
  if (Serial1.available())
    {
      val = Serial1.read();
          Serial.print("receiev from bluetooth  :") ;
         Serial.println(val, DEC);
    }
    delay(200);  

}


