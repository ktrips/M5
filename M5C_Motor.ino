#include<Wire.h>
#include<GroveI2CMiniMorter.h>
#include <M5StickC.h>

GroveI2CMiniMoto morter1(0x62);// Arduinoの場合は1ビット右シフトする
GroveI2CMiniMoto morter2(0x60);// Arduinoの場合は1ビット右シフトする
#define CONTRL 0x00 //レジスタのサブアドレス
#define FAULT 0x01 //レジスタのサブドレス

void setup() {
  Serial.begin(115200);
  Serial.println("Start Motor");
  Wire.begin();
  
 M5.begin();
 M5.Lcd.println("Hello Motor");
}

void loop() {
   Wire.write(CONTRL);
  Serial.println("Forward!");
   M5.Lcd.println("Fwd");

  morter1.forwardDrive(50);
  morter2.forwardDrive(50);

  delay(1000);
  Serial.println("Stop!");
  M5.Lcd.println("Stop");

  morter1.stop();
  morter2.stop();

  delay(1000);
  Serial.println("Reversal!");
  M5.Lcd.println("Reversal!");
  morter1.reversalDrive(50);
  morter2.reversalDrive(50);

  delay(1000);
  Serial.println("Stop!");
  M5.Lcd.println("Stop");
  morter1.stop();
  morter2.stop();

  delay(1000);
}
