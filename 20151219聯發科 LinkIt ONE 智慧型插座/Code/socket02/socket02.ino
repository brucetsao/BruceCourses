#define Aout A1
#define MidValue 524
void setup()
{
  pinMode(Aout,INPUT);
  Serial.begin(9600);
}

void loop()
{
  int Value = analogRead(Aout);
  double Angv= (double)(Value - MidValue) ;
   double Mv = Angv * 5000/1024 ;
   double Amp = Mv /60 ;
   double mAmp = Amp * 1000 ;
 // Serial.print("Aout Value =( ");
  Serial.print(Value);
  Serial.print("  ,  ");
  Serial.print(Angv);
  Serial.print("  ,  ");
  Serial.print(Mv);
  Serial.print("  ,  ");
  Serial.print(Amp) ;
  Serial.print("  ,  ");
  Serial.print(mAmp) ;
//  Serial.print(")\n");
  Serial.print("\n");
  //(30/1000) *1024/5
  delay(20);
}
