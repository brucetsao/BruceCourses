
#define RelayPin 2
void setup() {
  // put your setup code here, to run once:
  pinMode(RelayPin,OUTPUT);
    pinMode(13, OUTPUT);
 // digitalWrite(RelayPin,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
      digitalWrite(RelayPin,LOW);
      digitalWrite(13, LOW); 
      delay(300);
      digitalWrite(RelayPin,HIGH);
       digitalWrite(13, HIGH); 
      delay(300);
      
}
