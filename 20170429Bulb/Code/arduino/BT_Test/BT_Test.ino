
#include <SoftwareSerial.h>   // 引用程式庫

// 定義連接藍牙模組的序列埠
SoftwareSerial BT(6, 7); // 接收腳, 傳送腳
char val;  // 儲存接收資料的變數

void setup() {
 
  BT.begin(9600);
}

void loop() {

  // 若收到藍牙模組的資料，則送到「序列埠監控視窗」
      BT.println(millis()) ;
      delay(1000) ;
}
