#define relaypin1 13
#define turnon HIGH
#define turnoff LOW


void setup()
{
  pinMode(relaypin1, OUTPUT) ;


  Serial.begin(9600);
  Serial.println("program start here....");
}
void loop()
{
  digitalWrite(relaypin1, turnon);
  Serial.println("Now Switch Relay 1 On");
  delay(2000);
  digitalWrite(relaypin1, turnoff);
  Serial.println("Now Switch Relay 1 Off");
  delay(2000);

}

