#define SensorPin A1
double  MidValue = 522.4712 ;
#define ReadMaxCount 200
#define InitCurrentCount 200
#define initCount 11 
long initCounter = 0 ;
char getdata ;  // 儲存接收資料的變數
double Ampdata ;  // 儲存接收資料的變數
double s1 = 0 ;
double s2 = 0 ;
double s3 = 0 ;

void setup()
{
  pinMode(SensorPin,INPUT);
  Serial.begin(9600);
}

void loop()
{

//  Ampdata = ReadCurrentAverage() ;
  Ampdata = InitCurrentAverage() ;
  
 // Serial.print("Aout Value =( ");
  Serial.print(initCounter);
  Serial.print("  ,  ");
  Serial.print(Ampdata);
  Serial.print("  ,  ");
  Serial.print(Ampdata * 1000 );
  Serial.print("  ,  ");
  Serial.print(s1/ReadMaxCount );
  Serial.print("  ,  ");
  Serial.print(s2 /ReadMaxCount);
//  Serial.print("  ,  ");
//  Serial.print(s3 );
  Serial.print("\n");
  //(30/1000) *1024/5
  delay(200);
}


double ReadCurrentAverage()
{
     double Value;
  double Angv;
   double Mv;
   double Amp = 0 ;
   double mAmp ;
  for(int i=0 ;i <ReadMaxCount; i++)
    {
      Value = (double)analogRead(SensorPin);
     Angv= ((Value - MidValue) * 5000/1024) /60  ;
      Amp = Amp + (Angv * Angv) ;
    }
    
    return sqrt(Amp/ReadMaxCount) ;
}


double InitCurrentAverage()
{
  
     double Value;
  double Angv;
   double Mv;
   double Amp = 0 ;
   double mAmp ;
  s1 = 0 ;
  s2 = 0 ;
  s3 = 0 ;
  for(int i=0 ;i <InitCurrentCount; i++)
    {
      Value = (double)analogRead(SensorPin);
        if (initCounter >= initCount)
          {
                s1 = s1 + Value ;
               Angv= ((Value - MidValue) * 5000/1024) /60  ;
               s2 = s2 + Angv ;
                Amp = Amp + (Angv * Angv) ;
          }
    }
    initCounter ++ ;
    return sqrt(Amp/ReadMaxCount) ;
}
