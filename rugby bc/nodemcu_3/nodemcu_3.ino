#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <RFID.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
int buzzer = D0;


constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

String tag;

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  lcd.begin(16,2);
   lcd.setCursor(0,0);
   lcd.print("TAP THE");
   lcd.setCursor(0,1);
   lcd.print("RFID CARD");

}

void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "3610623543") {
      Serial.println("Access Granted!");
      lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Access Granted");
        lcd.setCursor(0,1);
        lcd.print("GATE OPEN");
        tone(buzzer, 4000,100);
        delay(1);
        delay(10000);
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("TAP THE");
        lcd.setCursor(0,1);
        lcd.print("RFID CARD");
     
    } else {
      Serial.println("Access Denied!");
       lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Access Denied");
        lcd.setCursor(0,1);
        lcd.print("GATE CLOSE");
  
        tone(buzzer,2000,100);
        delay(1);
        delay(10000);
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("TAP THE");
        lcd.setCursor(0,1);
        lcd.print("RFID CARD");
       
   
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
