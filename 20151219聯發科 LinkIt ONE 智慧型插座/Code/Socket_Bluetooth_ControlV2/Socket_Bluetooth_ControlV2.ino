#define SensorPin A1
#define RelayPin 13
#define ReadyPin 12
#define turnon HIGH
#define turnoff LOW
double  MidValue = 512 ;
#define ReadMaxCount 200
#define  InitCurrentCount 200      // calculate ananlog data counting for average data
#define initCount 10     // omit the initial unstable data
#define initAvgCount 5   //read new data for  MidValue
long initCounter = 0 ;
double s1 = 0 ;
double s2 = 0 ;
double s3 = 0 ;
char getdata ;  // 儲存接收資料的變數
double Ampdata ;  // 儲存接收資料的變數
long mAmpdata ;  // 儲存接收資料的變數

void setup() {
    pinMode(RelayPin , OUTPUT) ;  
    pinMode(ReadyPin , OUTPUT) ;  
    digitalWrite(RelayPin , turnon) ;  
    digitalWrite(ReadyPin , LOW) ;  
    digitalWrite(ReadyPin , HIGH) ;  
    delay(80) ;
    digitalWrite(ReadyPin , LOW) ;  
    delay(80) ;
    digitalWrite(ReadyPin , HIGH) ;  
    delay(80) ;
    digitalWrite(ReadyPin , LOW) ;  
    delay(80) ;
    digitalWrite(ReadyPin , HIGH) ;  
    delay(80) ;
    digitalWrite(ReadyPin , LOW) ;  
    delay(80) ;
 Serial.begin(9600);
 Serial1.begin(9600) ;
// while(!Serial) ;
 Serial.println("Program Start") ;
 initCounter= 0 ;
    Serial.println("now init Average") ;
  for(int i = 0 ; i <initCount; i++)
      {
          Ampdata = InitCurrentAverage() ; 
           debugCurrent()  ;
          initCounter  ++ ;
    }
      s3 = 0 ;  //recalculate init data
 initCounter= 0 ;
          Serial.println("now cal Average to set midValue") ;
   for(int i = 0 ; i <initAvgCount; i++)
      {
          Ampdata = InitCurrentAverage() ; 
           debugCurrent()  ; 
          initCounter  ++ ;
          s3= s3 +  (s1 / InitCurrentCount) ;
      }
        MidValue = (s3/initAvgCount) ;
  Serial.print("Now get new midValue:(");
  Serial.print(MidValue);
  Serial.print(")\n");
  
    Serial.println("now Start Detect Current") ;
    digitalWrite(ReadyPin , HIGH) ;  
        

}

//String analogValue0 = String(lightMeter.readLightLevel(), DEC);
void loop()
{
    
  getdata = '@'; 
  // 若收到藍牙模組的資料，則送到「序列埠監控視窗」
  if (Serial1.available()) {
    getdata = Serial1.read();
  
    Serial.print("(");
    Serial.print(getdata, DEC);
    Serial.print(")\n");

  }

    if (getdata == 67)
        {
          digitalWrite(RelayPin, turnoff) ;
        }

    if (getdata == 79)
        {
          digitalWrite(RelayPin, turnon) ;
        }

    if (getdata == 81)
        {
            Ampdata = ReadCurrentAverage() ;
            Serial1.print("@");
            Serial1.print(String(Ampdata,5) );
            Serial.print("(") ;
            Serial.print(Ampdata) ;
            Serial.print("/") ;
            Serial.print(String(Ampdata,5)) ;
            Serial.print(")\n") ;
            
        }

  
      delay(200) ;
  
}
//===============
//===============
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
   for(int i=0 ;i <InitCurrentCount; i++)
    {
      Value = (double)analogRead(SensorPin);
                s1 = s1 + Value ;
               Angv= ((Value - MidValue) * 5000/1024) /60  ;
               s2 = s2 + Angv ;
                Amp = Amp + (Angv * Angv) ;
          }
    return sqrt(Amp/ReadMaxCount) ;
}

void debugCurrent()
{
  
  
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

}


double ReadCurrent()
{
   int Value = analogRead(SensorPin);
  double Angv= (double)(Value - MidValue) ;
   double Mv = Angv * 5000/1024 ;
   double Amp = Mv /60 ;
   double mAmp = Amp * 1000 ;
 // Serial.print("Aout Value =( ");
 return Amp ;
}


