#include <SoftwareSerial.h>
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
  //Serial.begin(9600); // open serial connection
  mySerial.begin(9600);

}
void loop() {
  if (mfrc522.PICC_IsNewCardPresent()) { 
    mySerial.print("ID");
    if(mfrc522.PICC_ReadCardSerial()) { 
        for (byte i = 0; i < mfrc522.uid.size; ++i) { 
         
         mySerial.print(mfrc522.uid.uidByte[i],HEX);
      }
       mySerial.println();    
      }
      delay(1000);
    }
   delay(10); 
  }
  
