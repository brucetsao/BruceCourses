#define Aout A1
#define MidValue 524
void setup()
{
  pinMode(Aout,INPUT);
  Serial.begin(9600);
  while(!Serial) ;
}

void loop()
{
  int Value = analogRead(Aout);
  double Angv= (double)(Value - MidValue) ;
  Serial.print(Value);
  Serial.print("  ,  ");
  Serial.print(Angv);
  Serial.print("\n");
  //(30/1000) *1024/5
  
  delay(200);
}
