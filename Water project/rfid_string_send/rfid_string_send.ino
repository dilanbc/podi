#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "SPI.h" 
#include "MFRC522.h" 
const int pinRST = 9;
const int pinSDA = 10;
String tempstring1;
int enumber;

MFRC522 mfrc522(pinSDA, pinRST); // Set up mfrc522 on the Arduino
SoftwareSerial mySerial(7,6); // RX, TX
void setup() {
  
  SPI.begin(); // open SPI connection
  mfrc522.PCD_Init(); // Initialize Proximity Coupling Device (PCD)
  Serial.begin(9600); // open serial connection
  mySerial.begin(9600);
  tempstring1 = String();
}
void loop() {
  if (mfrc522.PICC_IsNewCardPresent()) { 
    if(mfrc522.PICC_ReadCardSerial()) { 
        for (byte i = 0; i < mfrc522.uid.size; ++i) { 
         
         tempstring1=tempstring1+(mfrc522.uid.uidByte[i]);
      }
      mySerial.println(tempstring1);
      Serial.println(tempstring1); // Print out of id is complete.
      for(int i=0;i<10;i=i+2){
       Serial.println(i);
       Serial.println((tempstring1.substring(i,i+2)).toInt());
       EEPROM.write(i/2,(tempstring1.substring(i,i+2)).toInt()); 
      }
   
      
      tempstring1=String();
      delay(1000);
    }
  }
  delay(10);
}
