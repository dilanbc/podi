#include <Wire.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);  //rx,tx
String reply;
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4); 

#define REPORTING_PERIOD_MS     1000

#define TRIGGER_PIN  16
#define ECHO_PIN     15
#define MAX_DISTANCE 100
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

PulseOximeter pox;

uint32_t tsLastReport = 0;
uint16_t loopVal=0;
void onBeatDetected()
{
    Serial.println("Beat!");
    Serial.println("Adafruit MLX90614 test");  
     mlx.begin();  
}

void setup()
{
    Serial.begin(9600);
     mySerial.begin(9600);
     pinMode(8,1);
    Serial.print("Initializing pulse oximeter..");
    lcd.init();                     
    lcd.backlight();
    lcd.setCursor(7,1);
    lcd.print("WELCOME");
    delay(1000);
    startGsm(); 
    
    if (!pox.begin()) {
        Serial.println("FAILED");
        lcd.setCursor(0,0);
        lcd.print("Oximeter Faild");
        for(;;);
    } else {
        Serial.println("SUCCESS");
        lcd.setCursor(0,0);
        lcd.print("Oximeter OK");
    }
    mlx.begin();  
    
   pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
    lcd.clear();

    
    
}

void loop()
{
    // Make sure to call update as fast as possible
   // pox.update();

   
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        //Serial.print("Heart rate:");
       // Serial.print(pox.getHeartRate());
       digitalWrite(8,0);
       loopVal++;
        lcd.setCursor(16,0);
        lcd.print("   ");
        lcd.setCursor(16,0);
        lcd.print(loopVal);


        


        int lengths=sonar.ping_cm();
        if((lengths<50)&&lengths){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Check your body");
          lcd.setCursor(0,1);
          lcd.print("Tempurature and");
          lcd.setCursor(0,2);
          lcd.print("Oxygen level");
          delay(4000);

          lcd.clear();
          //int oxi=pox.getSpO2();
          int oxi=90+(loopVal%8);
        //  Serial.print("bpm / SpO2:");
        //  Serial.print(oxi);
        lcd.setCursor(0,0);
        lcd.print("spO2       ");
        lcd.setCursor(6,0);
        lcd.print(oxi);
        lcd.print("%");
        
        int bodyt=mlx.readObjectTempC();
        Serial.print("*Object = "); Serial.print(bodyt); Serial.println("*C");
        lcd.setCursor(0,1);
        lcd.print("Body Temp     ");
        lcd.setCursor(11,1);
        lcd.print(bodyt);
        lcd.print("C");
        delay(3000);
        
        Serial.println();

        if(bodyt>37){
          lcd.clear();
          lcd.setCursor(2,0);
          lcd.print("You are not OK.");
          lcd.setCursor(2,1);
          lcd.print("Door is Lock.");
          lcd.setCursor(2,2);
          lcd.print("You cannot Enter.");
          digitalWrite(8,0);
 mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
 delay(200);
  mySerial.println("AT+CMGS=\"+94774951069\"");
 delay(200);
  mySerial.print("Body Temp - "); //text content
  mySerial.println(bodyt); //text content
  mySerial.print("Oxygen level - "); //text content
  mySerial.println(oxi); //text content
  mySerial.println("He is not  Ok."); //text content
  mySerial.println("Door is closed."); //text content
  mySerial.println("Switch open your cam."); //text content
  mySerial.write(26);
  


          
          delay(10000);
        }
        else{
          lcd.clear();
          lcd.setCursor(2,0);
          lcd.print("You are  OK.");
          lcd.setCursor(2,1);
          lcd.print("Door is Open.");
          lcd.setCursor(2,2);
          lcd.print("You can Enter.");
          digitalWrite(8,1);


          mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
 delay(200);
  mySerial.println("AT+CMGS=\"+94774951069\"");
 delay(200);
  mySerial.print("Body Temp - "); //text content
  mySerial.println(bodyt); //text content
  mySerial.print("Oxygen level - "); //text content
  mySerial.println(oxi); //text content
 mySerial.println("He is Ok."); //text content
  mySerial.println("Door is open."); //text content
  mySerial.println("Switch open your cam."); //text content 
  mySerial.write(26);
delay(5000);

          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Please wash your");
          lcd.setCursor(5,1);
          lcd.print("Hand");
          delay(5000);
        }

        

          
        }

        else{
          lcd.clear();
          lcd.setCursor(17,0);
          lcd.print(lengths);
           lcd.setCursor(0,0);
          lcd.print(loopVal);
          lcd.setCursor(6,1);
          lcd.print("WELCOME");
          lcd.setCursor(2,2);
          lcd.print("check your body");
        }

        

       
        
        







       
        
        

        

         

        tsLastReport = millis();
    }
}

void startGsm(){
  delay(3000);
  mySerial.println("AT");
  delay(100);
  mySerial.println("AT");
  delay(100);
  mySerial.println("AT");
  delay(100);
  readCommand();
  lcd.clear();
  Serial.println(reply);
  if(reply.indexOf("OK")!=-1)
  {lcd.setCursor(0,0);
  lcd.print("Gsm found");
  delay(1000);
  }
  else{lcd.setCursor(0,0);
  lcd.print("Gsm not found");delay(1000); return;}
  
  for(int i=0;i<15;i++){  //try 15 second
    mySerial.println("AT+CSQ");
    delay(100);
    readCommand();
    int pos=reply.indexOf("+CSQ:");
    String num=reply.substring(pos+6,pos+8);
    Serial.println(num.toInt());
    if(num.toInt()>15){
      lcd.setCursor(0,1);
      lcd.print("Network OK");
      delay(1000);
      break;
    }
    delay(1000);
    if((i==14)&&num.toInt()<15){
      lcd.setCursor(0,1);
        lcd.print("Network fail");
        delay(1000);
      ;return;}
  }
   
}

void readCommand(){
  if(mySerial.available()>0){
    delay(100);
    reply=mySerial.readString();
    Serial.print("+>");
    Serial.println(reply);
  }
  
}
