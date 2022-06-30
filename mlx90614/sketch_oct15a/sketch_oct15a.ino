#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4); 

#define REPORTING_PERIOD_MS     1000

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
    Serial.print("Initializing pulse oximeter..");
    lcd.init();                     
    lcd.backlight();
    lcd.setCursor(7,1);
    lcd.print("WELCOME");
    delay(1000);
    lcd.clear();
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
    
    pox.setOnBeatDetectedCallback(onBeatDetected);
    delay(500);
    lcd.clear();
}

void loop()
{
    // Make sure to call update as fast as possible
    pox.update();

   
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        //Serial.print("Heart rate:");
       // Serial.print(pox.getHeartRate());
       loopVal++;
        lcd.setCursor(17,0);
        lcd.print("   ");
        lcd.setCursor(17,0);
        lcd.print(loopVal);

        
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        lcd.setCursor(0,0);
        lcd.print("spO2     ");
        lcd.setCursor(6,0);
        lcd.print(pox.getSpO2());
        lcd.print("%");
        //Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
      //  Serial.println();

        tsLastReport = millis();
    }
}
