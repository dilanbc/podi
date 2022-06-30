#include <SoftwareSerial.h>
SoftwareSerial mySerial(6,7);  //tx,rx of module
#include <SPI.h>
#include <SD.h>
File myFile;

String reading;

void setup() 
{
  
  Serial.begin(9600);
  mySerial.begin(9600);
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("press 1 for install card");
  
}

void loop() {
  
  if(mySerial.available()>0){
    reading = mySerial.readString();
    if(reading.indexOf("ID")>=0){
     reading=reading.substring(reading.indexOf("ID")); 
     reading=reading.substring(2,10);
     Serial.println(reading);
     }
     if(SD.exists(reading+".txt")){
     Serial.println("Correct ID");
    }
    if(!SD.exists(reading+".txt")){
     Serial.println("Invaild ID"); 
    }
     
    delay(1000);
    
  }
  if (Serial.available() > 0) {
    reading = Serial.readString();
    Serial.println(reading);
    if(reading.indexOf("1")>=0){
     installcard(); 
    }
  }
   
  
}



void installcard(){
 Serial.println("Tap the card");
  
}

