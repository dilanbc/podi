#include <LoRa.h>
#include <SPI.h>

#include "LCDIC2.h"

LCDIC2 lcd(0x27, 16, 2);

#define ss 5
#define rst 16
#define dio0 17

unsigned int notcount=0;

 
void setup() 
{
  delay(500);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("welcome");
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
 
  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xA5);
  Serial.println("LoRa Initializing OK!");
  delay(2000);
  lcd.clear();
}
 
void loop() 
{
  int packetSize = LoRa.parsePacket();    // try to parse packet
  if (packetSize) 
  {
    notcount=0;
    Serial.print("Received packet '");
 
    while (LoRa.available())              // read packet
    {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 

      int value=LoRaData.toInt();
      
       lcd.setCursor(0,1);
       lcd.print("New order -   ");
       lcd.setCursor(12,0);
       lcd.print(LoRaData);
  
   
        
     }
      
    }
    Serial.print("' with RSSI ");         // print RSSI of packet
    Serial.println(LoRa.packetRssi());
  }
