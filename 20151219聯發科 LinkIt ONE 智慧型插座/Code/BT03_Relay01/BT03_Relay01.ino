
#define RelayPin 13
#define turnon HIGH
#define turnoff LOW

// 定義連接藍牙模組的序列埠

char getdata ;  // 儲存接收資料的變數


void setup() {
  pinMode(RelayPin , OUTPUT) ;    // set Button pin as input data
   digitalWrite(RelayPin, turnon) ;

  Serial.begin(9600);   // 與電腦序列埠連線
  Serial.println("BT is ready!");
  
  // 設定藍牙模組的連線速率
  // 如果是HC-05，請改成38400
  Serial1.begin(9600);   // 與電腦序列埠連線
}

void loop() {
     
    getdata = '@'; 
  // 若收到藍牙模組的資料，則送到「序列埠監控視窗」
  if (Serial1.available()) {
    getdata = Serial1.read();
    Serial.print(getdata, DEC);
  }

    if (getdata == '0')
        {
          digitalWrite(RelayPin, turnoff) ;
        }

    if (getdata == '1')
        {
          digitalWrite(RelayPin, turnon) ;
        }
}

