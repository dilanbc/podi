#include <SoftwareSerial.h>
SoftwareSerial mySerial(8,9);  //tx,rx of module
String textMessage;
int resetport;

void setup() 
{
  Serial.begin(9600);
  Serial.print("Starting");
  mySerial.begin(9600);
 mySerial.println("AT");
 delay(50);
 updateSerial(); 
 mySerial.println("AT+CMGF=1");
 delay(50);
 updateSerial();
 for(int i=1;i<50;i++){
  mySerial.print("AT+CMGD=");
  mySerial.println(i);
 delay(50);
 updateSerial();
 }
 mySerial.println("AT+CNMI=2,2,0,0,0");
 delay(50);
 updateSerial();
 
}

void loop() {
if(mySerial.available()>0){
    //updateSerial();
    //Serial.println("  "); 
    textMessage = mySerial.readString();
    Serial.print(textMessage);    
    delay(10);
  } 
  if(textMessage.indexOf("RESET1")>=0){
    Serial.println("Port 1 Reset");  
    textMessage = ""; 
    resetport=1;
    replymessage();
      
  }
  if(textMessage.indexOf("RESET2")>=0){
    Serial.println("Port 2 Reset");  
    textMessage = ""; 
    resetport=2;
    replymessage();  
  }
  if(textMessage.indexOf("RESET3")>=0){
    Serial.println("Port 3 Reset");  
    textMessage = "";
    resetport=3;
    replymessage();   
  }
  if(textMessage.indexOf("RESET4")>=0){
    Serial.println("Port 4 Reset");  
    textMessage = "";
    resetport=4;
    replymessage();   
  }
  if(textMessage.indexOf("RESET5")>=0){
    Serial.println("Port 5 Reset");  
    textMessage = "";
    resetport=5;
    replymessage();   
  }
  if(textMessage.indexOf("RESET1")>=0){
    Serial.println("Port 6 Reset");  
    textMessage = ""; 
    resetport=6;
    replymessage();  
  }
delay(100);  
}  


void updateSerial(){
  delay(50);
 while(mySerial.available()) {
    Serial.write(mySerial.read());
  }
  while (Serial.available()) {
    mySerial.write(Serial.read());
  } 
}

void replymessage(){
  
  mySerial.print("AT+CMGF=1\r"); 
  delay(100);
  
  mySerial.println("AT+CMGS=\"+94713673133\""); 
  delay(100);
  mySerial.print("Port ");
  mySerial.print(resetport);
  mySerial.println(" reset .");
  mySerial.println((char)26); 
  delay(100);
  mySerial.println();
  delay(5000);  
}
