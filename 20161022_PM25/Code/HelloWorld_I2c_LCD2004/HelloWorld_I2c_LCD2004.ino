//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

 LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // 設定 LCD I2C 位址

void setup()
{
  lcd.begin(20, 4);      // 初始化 LCD，一行 20 的字元，共 4 行，預設開啟背光
  lcd.backlight(); // 開啟背光
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
   lcd.setCursor(2,1);
  lcd.print("By BruceTsao!");
}


void loop()
{
    lcd.setCursor(3,0);
  lcd.print("Hello, world!");
   lcd.setCursor(2,1);
  lcd.print("By BruceTsao!");
  delay(2000) ;
}
