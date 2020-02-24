#include <M5StickC.h>
#include <Wire.h>
#include <RPR-0521RS.h>

RPR0521RS rpr0521rs;

void setup() {
  byte rc;
  
  M5.begin();
  Serial.begin(115200);
  while (!Serial);
  
  Wire.begin(0,26);

  M5.Lcd.setTextSize(2);

  rc = rpr0521rs.init();
}

void loop() {
  Wire.begin(0,26);

  byte rc;
  unsigned short ps_val;
  float als_val;
  byte near_far;

  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(10, 20);
      
  rc = rpr0521rs.get_psalsval(&ps_val, &als_val);
  if (rc == 0) {
    Serial.print(F("RPR-0521RS (Proximity)     = "));
    M5.Lcd.printf("RPR-0521RS: ");
    Serial.print(ps_val);
    Serial.print(F(" [count]"));
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
    }
  }
 
  delay(500);

}
