#define SensorPin 9
int value ;
void setup() {
  // put your setup code here, to run once:
    pinMode(SensorPin,INPUT);
    Serial.begin(9600);
}

void loop() {
  value = digitalRead(SensorPin)  ;
  // put your main code here, to run repeatedly:
    Serial.print("Sensor Status is :(");
    Serial.print(value) ;
    Serial.print(")\n");
    delay(800);
}
