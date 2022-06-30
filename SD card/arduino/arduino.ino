#include <SD.h>                      // need to include the SD library
//#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#define SD_ChipSelectPin 10  //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3 );  //rx,tx
String reply;
TMRpcm tmrpcm;   // create an object for use in this sketch


void setup(){

  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc

  Serial.begin(9600);
  mySerial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
Serial.setTimeout(300);
startGsm();  
call();
  
}



void loop(){  
if(mySerial.available()>0) {
   delay(100);
    reply=mySerial.readString();
    Serial.print("->");
    Serial.println(reply);
  }
  while(Serial.available()) {
    mySerial.write(Serial.read());
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
void startGsm(){
  delay(1000);
  mySerial.println("AT");
  delay(100);
  readCommand();
  if(reply.indexOf("OK")!=-1)
  {Serial.println("Gsm found");    
  }
  else{Serial.println("Gsm notfound"); return;}
  
  for(int i=0;i<15;i++){  //try 15 second
    mySerial.println("AT+CSQ");
    delay(100);
    readCommand();
    int pos=reply.indexOf("+CSQ:");
    String num=reply.substring(pos+6,pos+8);
    Serial.println(num.toInt());
    if(num.toInt()>15){
      Serial.println("Network ok");
      break;
    }
    delay(1000);
    if((i==14)&&num.toInt()<15){Serial.println("network fail");return;}
  }
   mySerial.println("AT+DDET=1,0,0");//enable dtmf
   delay(100);
  readCommand();
  mySerial.println("AT+COLP=1");//enable call answere notification
   delay(100);
  readCommand();
}

void call(){
  delay(1000);
  reply="";
  mySerial.println("ATD+94712505005;");
  delay(100);
  readCommand();
  while(reply.indexOf("+COLP:")==-1){
    delay(100);
    readCommand();
   }
  Serial.println("Play");
  delay(500);
  tmrpcm.play("test.wav"); //the sound file "music" will play each time the arduino powers up, or is reset
  while(1){
    if(reply.indexOf("+DTMF: 2")!=-1){
      delay(1000);
      mySerial.println("atd+94713673133;");
      readCommand();
      while(reply.indexOf("+COLP:")==-1){
    delay(100);
    readCommand();
    }
    delay(100);
    mySerial.println("AT+CHLD=3");
    
    readCommand();
      break;
    }
    if(reply.indexOf("+DTMF: 1")!=-1){
      mySerial.println("ATH");
      break;
    }
    delay(100);
    readCommand();
  }
  
}
