#include <LoRa.h>
#include <SPI.h>
#include "LCDIC2.h"
LCDIC2 lcd(0x27, 16, 2);
 
#define ss 5
#define rst 16
#define dio0 17

#define upkey 2
#define downkey 4
#define selectkey 15

#define tablelimit 5
int tablecount=0;


int counter = 0;

char lcddata[30];

#define foodlim 12
String foodst [foodlim]={"Baked Crab","Lava Cake","Chicken ","Sandwitches","Seafood ","Fried rice","Chopsy rice","Cuttlets","Burger  ","Cheese pasta","Submarine","Noodles "};
int foodpr [foodlim]={1500,1890,1200,1350,2300,1800,1500,190,1200,1450,1300,1650}; 
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
lcd.print("Welcome to");
lcd.setCursor(2,1);
lcd.print("Food Buffet");
delay(3000);

showtable();





  
}
 
void loop() 
{


  

  if(digitalRead(upkey)){
    tablecount++;
    if(tablecount>tablelimit-1){
      tablecount=0;
    }
   showtable();
  }

    if(digitalRead(downkey)){
    tablecount--;
    if(tablecount<0){
      tablecount=tablelimit-1;
     }
     showtable();
  }
  if(digitalRead(selectkey)){
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Table selected");
   delay(500);
   lcd.clear();
  showdata();
  while(1){
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
  showtable();
  break;
 }
  

  
   
  }  
  } 


///////////////////////////////////////////////////////////////////////////reply data
int packetSize = LoRa.parsePacket();    // try to parse packet
  if (packetSize) 
  {
 
    Serial.print("Received packet '");
 
    while (LoRa.available())              // read packet
    {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
      int replydata=LoRaData.toInt();
      if(replydata){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Order Available!");
        lcd.setCursor(0,1);
        lcd.print("Please Wait..");
      }
      else{
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("We are Sorry");
       lcd.setCursor(0,1);
       lcd.print("Select Another"); 
      }
     delay(4000);
    }
    Serial.println(LoRa.packetRssi());
   showtable();  
  }

  


 
}

void showdata(){
sprintf(lcddata,"%02d %.8s %d",counter+1,foodst[counter],foodpr[counter]);
    lcd.setCursor(0,0);
    lcd.print(lcddata);

    if(counter>=foodlim-1){
     lcd.setCursor(0,1);
     lcd.print("                "); 
    }
    else{
     sprintf(lcddata,"%02d %.8s %d",counter+2,foodst[counter+1],foodpr[counter+1]);
    lcd.setCursor(0,1);
    lcd.print(lcddata); 
    }
    
    delay(500);
}


void showtable(){
lcd.clear();
lcd.setCursor(0,0);

lcd.print("Enter Table No:");
lcd.setCursor(0,1);
lcd.print(tablecount+49);//ascii
delay(500);
  
}


void senddata(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sending data. .");

  
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print(((tablecount+1)*100)+(counter+1));
  LoRa.endPacket();
  
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Data sent.");
  delay(2000);
  lcd.clear();


 

  
}
