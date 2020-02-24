#include <M5StickC.h>
#include <Wire.h>
#include <RPR-0521RS.h>

RPR0521RS rpr0521rs;

#include<GroveI2CMiniMorter.h>

GroveI2CMiniMoto morter1(0x62);// Arduinoの場合は1ビット右シフトする
GroveI2CMiniMoto morter2(0x60);// Arduinoの場合は1ビット右シフトする
#define CONTRL 0x00 //レジスタのサブアドレス
#define FAULT 0x01 //レジスタのサブドレス


void setup() {
  byte rc;
  
  Serial.begin(115200);
  while (!Serial);
  
  M5.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setRotation(3);

  Wire.begin(0,26);
  rc = rpr0521rs.init();

  Wire.begin(32,33);
  Serial.println("Start Motor");
}

void loop() {
  //Blynk.run();
  
  Wire.begin(0,26);
  byte rc;
  unsigned short ps_val;
  float als_val;
  byte near_far;

  M5.Lcd.setCursor(10, 20);
      
  rc = rpr0521rs.get_psalsval(&ps_val, &als_val);
  if (rc == 0) {
    Serial.print(F("RPR-0521RS (Proximity)     = "));
    M5.Lcd.printf("RPR-0521RS: ");
    Serial.print(ps_val);
    Serial.print(F(" [count]"));
        
    Wire.begin(32,33);
    Wire.write(CONTRL);

    if (ps_val > 60) {
      Serial.println(F(" Danger!"));
      M5.Lcd.printf(" Danger!");

      morter1.stop();
      morter2.stop();
      morter1.reversalDrive(50);
      morter2.reversalDrive(50);
      delay(200);
      morter1.forwardDrive(50);
      morter2.reversalDrive(50);
      delay(100);
    } else {
      Serial.println(F(" Go!Go!"));
      M5.Lcd.printf(" Go!Go!");

      morter1.forwardDrive(50);
      morter2.forwardDrive(50);
      delay(300);
      morter1.stop();
      morter2.stop();
    }

    /**
    near_far = rpr0521rs.check_near_far(ps_val);
    if (near_far == RPR0521RS_NEAR_VAL) {
      Serial.println(F(" Near"));
      M5.Lcd.printf(" Near!");
    } else {
      Serial.println(F(" Far"));
      M5.Lcd.printf(" Far !");
    }
    
    M5.Lcd.setCursor(10, 60);

    if (als_val != RPR0521RS_ERROR) {
      Serial.print(F("RPR-0521RS (Ambient Light) = "));
      Serial.print(als_val);
      Serial.println(F(" [lx]"));
          
      if (als_val < 10) {
         M5.Lcd.printf("Dark !");
      } else {
         M5.Lcd.printf("Bright");
      }
      Serial.println();
    }**/
  }
  
  delay(300);

}
