#define SensorPin A1
#define MidValue 524
#define ReadMaxCount 200

char getdata ;  // 儲存接收資料的變數
double Ampdata ;  // 儲存接收資料的變數

void setup()
{
  pinMode(SensorPin,INPUT);
  Serial.begin(9600);
}

void loop()
{

  Ampdata = ReadCurrentAverage() ;
 // Serial.print("Aout Value =( ");
  Serial.print(Ampdata);
  Serial.print(" Amp ,  ");
  Serial.print(Ampdata);
  Serial.print("mA ");
  Serial.print("\n");
  //(30/1000) *1024/5
  delay(200);
}


double ReadCurrentAverage()
{
     int Value;
  double Angv;
   double Mv;
   double Amp = 0 ;
   double mAmp ;

  for(int i=0 ;i <ReadMaxCount; i++)
    {
      Value = analogRead(SensorPin);
     Angv= ((double)(Value - MidValue) * 5000/1024) /60  ;
      Amp = Amp + (Angv * Angv) ;
    }
    
    return sqrt(Amp/ReadMaxCount) ;
}
