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
  lcd.setCursor(0);
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
 
  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xA5);
  Serial.println("LoRa Initializing OK!");
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
      if(value>=0){
        lcd.setCursor(0, 0);
       lcd.print("Connected    ");

       if(value>0){
       lcd.setCursor(0,1);
       lcd.print("New order -   ");
       lcd.setCursor(12,1);
       lcd.print(LoRaData);
       }
       else{
        
       }
        
     }
      
    }
    Serial.print("' with RSSI ");         // print RSSI of packet
    Serial.println(LoRa.packetRssi());
  }
  else{
    notcount++;
    if(notcount>60000){
     lcd.setCursor(0, 0);
   lcd.print("Not Connected");  
   notcount=60001;
    }
   
  }
if(digitalRead(4)){
  lcd.setCursor(0,1);
   lcd.print("                ");
   delay(1000);  
}


 int packetSize = LoRa.parsePacket();    // try to parse packet
  if (packetSize) 
  {
 
    Serial.print("Received packet '");
 
    while (LoRa.available())              // read packet
    {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
    }
  }
}
