#include <LoRa.h>
#include <SPI.h>
#include "LCDIC2.h"
LCDIC2 lcd(0x27, 20,4);
 
#define ss 5
#define rst 16
#define dio0 17

#define upkey 2
#define downkey 15
#define selectkey 4


int counter = 0;

char lcddata[30];

#define foodlim 10
String foodst [foodlim]={"Special Rice","Chicken Mix.","Seafood Mix. ","Nasi Goreng ","Chicken Bir.","Chicken Kot.","Seafood Kot.","Cheese Kott.","Chicken Nood ","Seafood Nood"};
int foodpr [foodlim]={1200,650,900,1000,850,550,800,950,700,950}; 
void setup() 
{
  Serial.begin(115200); 
  while (!Serial);
  Serial.println("LoRa Sender");
  lcd.begin();
  lcd.setCursor(0);//off cursor
  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
  
  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xA5);
  Serial.println("LoRa Initializing OK!");

lcd.clear();
lcd.setCursor(3,0);
lcd.print("WELCOME To");
lcd.setCursor(2,1);
lcd.print("HOGWARTS CAFE");
delay(3000);






  
   showdata();
}
 
void loop() 
{
    if(digitalRead(upkey)){
    counter++;
    if(counter>foodlim-1){
      counter--;
    }
    showdata();
  }

    if(digitalRead(downkey)){
    counter--;
    if(counter<0){
      counter++;
    }
    showdata();
  }
 if(digitalRead(selectkey)){
  
  senddata();

  
 }
  

  
   
 
  


///////////////////////////////////////////////////////////////////////////reply data
int packetSize = LoRa.parsePacket();    // try to parse packet
  if (packetSize) 
  {
  int replydata;
    Serial.print("Received packet '");
 
    while (LoRa.available())              // read packet
    {
      String LoRaData = LoRa.readString();
     // Serial.println(LoRaData); 
      replydata=LoRaData.toInt();
      Serial.println(replydata);
    }LoRa.packetRssi();
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Order Update");
    sprintf(lcddata,"Order Number-%02u",replydata/10);
    lcd.setCursor(0,1);
    lcd.print(lcddata);
    lcd.setCursor(0,2);
    switch(replydata%10){
      case 0:lcd.print("No update");break; 
      case 1:lcd.print("Order Ready");break; 
      case 2:lcd.print("Please wait");break; 
      case 3:lcd.print("Sorry try another");break; 
    }
  delay(6000);
  lcd.clear();
  showdata();
  }

  


 
}

void showdata(){

lcd.setCursor(3,0);
lcd.print("Select meal");
  
sprintf(lcddata,"%02d>%.12s %4d",counter+1,foodst[counter],foodpr[counter]);
    lcd.setCursor(0,1);
    lcd.print(lcddata);

if(counter>foodlim-2){
  lcd.setCursor(0,2);
  lcd.print("                    ");
}
else{
sprintf(lcddata,"%02d %.12s %4d",counter+2,foodst[counter+1],foodpr[counter+1]);
    lcd.setCursor(0,2);
    lcd.print(lcddata);  
}



if(counter>foodlim-3){
  lcd.setCursor(0,3);
  lcd.print("                    ");
}
else{
 sprintf(lcddata,"%02d %.12s %4d",counter+3,foodst[counter+2],foodpr[counter+2]);
    lcd.setCursor(0,3);
    lcd.print(lcddata);   
}

  
    




    
    
    
    delay(500);
}




void senddata(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sending data. .");

  LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket(); LoRa.beginPacket();   
  LoRa.print(counter+1);
  LoRa.endPacket();
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Data sent.");
  delay(2000);
  lcd.clear();
  showdata();

 

  
}
