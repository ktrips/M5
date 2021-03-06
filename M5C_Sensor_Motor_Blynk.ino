#include <M5StickC.h>
#include<Wire.h>

#include<GroveI2CMiniMorter.h>
GroveI2CMiniMoto motor1(0x62);// Arduinoの場合は1ビット右シフトする
GroveI2CMiniMoto motor2(0x60);// Arduinoの場合は1ビット右シフトする
#define CONTRL 0x00 //レジスタのサブアドレス
#define FAULT 0x01 //レジスタのサブドレス

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <M5StickC.h>

// You should get Auth Token in the Blynk App.
char auth[] = "xxxx"; //BlynkアプリのYourAuthTokenを入力";
// Your WiFi credentials.
char ssid[] = "yyyy";
char pass[] = "zzzz";

long Speed;
long Speed1, Speed2;
long SpeedR, SpeedL;

#include <RPR-0521RS.h>
RPR0521RS rpr0521rs;

//ジョイスティックのデータ受信
BLYNK_WRITE(V0) {

  Wire.write(CONTRL);
  
  long x = param[0].asInt();
  long y = param[1].asInt();
 
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("  y: ");
  Serial.print(y);

  Speed = sqrt(x*x+y*y);
  if(Speed > 50){
    Speed = 50;
  }
  Serial.print(" Speed: ");
  Serial.print(Speed);

  SpeedR = Speed - Speed1;
  SpeedL = Speed - Speed2;
  if(x >= 0){
      Speed1 = abs(x);
      Speed2 = 0;
  }else{
      Speed1 = 0;
      Speed2 = abs(x);
  }
  Serial.print("  R:");
  Serial.print(SpeedR);
  Serial.print("  L:");
  Serial.println(SpeedL);  
  Blynk.virtualWrite(V5, SpeedR);
  Blynk.virtualWrite(V6, SpeedL);


  if(y >= 0){
    motor1.forwardDrive(SpeedR); 
    motor2.forwardDrive(SpeedL);
    M5.Lcd.println("Fwd!");
        
  }else{
    motor1.reversalDrive(SpeedR);
    motor2.reversalDrive(SpeedL);
    M5.Lcd.println("Back!");    

  }
  delay(500);
  motor1.stop();
  motor2.stop();
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Start Motor");
  Blynk.begin(auth, ssid, pass);
  Wire.begin(32, 33);
  
  M5.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setRotation(3);
  
  M5.Lcd.println("Start Motor");
  M5.Lcd.println(ssid);
  
  byte rc;
  Wire.begin(0,26);
  rc = rpr0521rs.init();
  Serial.println("Start RC Sensor");
}

void loop() {
  Blynk.run();

  Wire.begin(0,26);
  byte rc;
  unsigned short ps_val;
  float als_val;
      
  rc = rpr0521rs.get_psalsval(&ps_val, &als_val);
  if (rc == 0) {
    Serial.print(ps_val);
    Serial.print(F(" [count]"));

    if (als_val != RPR0521RS_ERROR) {
      Serial.print(als_val);
      Serial.println(F(" [lx]"));
      Blynk.virtualWrite(V7, round(als_val));
    }
        
    Wire.begin(32,33);
    Wire.write(CONTRL);

    M5.Lcd.setCursor(10, 60);

    if (ps_val > 60) {
      Serial.println(F(" Danger!"));
      M5.Lcd.printf(" Danger!");

      motor1.reversalDrive(50);
      motor2.reversalDrive(50);
      delay(300);
      motor1.forwardDrive(50);
      motor2.reversalDrive(50);
      delay(500);
      motor1.stop();
      motor2.stop();
      
    } else {
      Serial.println(F(" Go!Go!"));
      M5.Lcd.printf(" Go!Go!");
      
    }
  }
  delay(500);
}
