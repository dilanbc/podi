#include <EEPROM.h>
#include <SoftwareSerial.h>

int port1=2;
int port2=3;
int port3=4;
int port4=5;
int port5=6;
int port6=7;
String port1val;
String port2val;
String port3val;
String port4val;
String port5val;
String port6val;
String adminnum;
SoftwareSerial mySerial(12, 9);
String textMessage;
String temp;
int num;
void setup() {

  pinMode(port1,OUTPUT);
  pinMode(port2,OUTPUT);
  pinMode(port3,OUTPUT);
  pinMode(port4,OUTPUT);
  pinMode(port5,OUTPUT);
  pinMode(port6,OUTPUT);  
  pinMode(11,OUTPUT);
    for(int i=1;i<7;i++){
    digitalWrite(i+1,EEPROM.read(i));
    delay(30);
  }
  
  Serial.begin(9600); 
  mySerial.begin(9600);
  digitalWrite(11,1);
  delay(100);
  digitalWrite(11,0);
  delay(10000);
  Serial.print( "Starting...");
  mySerial.println("AT");
  delay(50);
  mySerial.println("AT+CMGF=1");
  delay(50);

  mySerial.println("AT+CNMI=1,2,0,0,0");
  delay(50);
  for(int i=1;i<50;i++){
  mySerial.print("AT+CMGD=");
  mySerial.println(i);
  delay(50);
  }

 sendSMS();
}

void loop(){
  if(mySerial.available()>0){
    delay(100);
    textMessage = mySerial.readString();
    delay(50);
    Serial.print(textMessage);  
      
    delay(10);
   
  if((textMessage.indexOf("1ON")>=0)or(textMessage.indexOf("1on")>=0)or(textMessage.indexOf("1 On")>=0)){
   EEPROM.write(1,1);
   delay(20);
   digitalWrite(port1,HIGH);
   textMessage = "";
    sendSMS();
  }
  if((textMessage.indexOf("1OFF")>=0)or(textMessage.indexOf("1off")>=0)or(textMessage.indexOf("1 Off")>=0)){
   EEPROM.write(1,0);
   delay(20);
   digitalWrite(port1,LOW);
   textMessage = "";
    sendSMS();
  }
  if((textMessage.indexOf("2ON")>=0)or(textMessage.indexOf("2on")>=0)or(textMessage.indexOf("2 On")>=0)){
   EEPROM.write(2,1);
   delay(20);
   digitalWrite(port2,HIGH);
   textMessage = "";
    sendSMS();
  }
  if((textMessage.indexOf("2OFF")>=0)or(textMessage.indexOf("2off")>=0)or(textMessage.indexOf("2 Off")>=0)){
   EEPROM.write(2,0);
   delay(20);
   digitalWrite(port2,LOW);
   textMessage = "";
    sendSMS();
  }
  if((textMessage.indexOf("3ON")>=0)or(textMessage.indexOf("3on")>=0)or(textMessage.indexOf("3 On")>=0)){
   EEPROM.write(3,1);
   delay(20);
   digitalWrite(port3,HIGH);
   textMessage = "";
   sendSMS();
  }
  if((textMessage.indexOf("3OFF")>=0)or(textMessage.indexOf("3off")>=0)or(textMessage.indexOf("3 Off")>=0)){
   EEPROM.write(3,0);
   delay(20);
   digitalWrite(port3,LOW);
   textMessage = "";
    sendSMS();
  }
  if((textMessage.indexOf("4ON")>=0)or(textMessage.indexOf("4on")>=0)or(textMessage.indexOf("4 On")>=0)){
   EEPROM.write(4,1);
   delay(20);
   digitalWrite(port4,HIGH);
   textMessage = "";
    sendSMS();
  }
  if((textMessage.indexOf("4OFF")>=0)or(textMessage.indexOf("4off")>=0)or(textMessage.indexOf("4 Off")>=0)){
   EEPROM.write(4,0);
   delay(20);
   digitalWrite(port4,LOW);
   textMessage = "";
    sendSMS();
  }
  if((textMessage.indexOf("5ON")>=0)or(textMessage.indexOf("5on")>=0)or(textMessage.indexOf("5 On")>=0)){
   EEPROM.write(5,1);
   delay(20);
   digitalWrite(port5,HIGH);
   textMessage = "";
  sendSMS();
  }
  if((textMessage.indexOf("5OFF")>=0)or(textMessage.indexOf("5off")>=0)or(textMessage.indexOf("5 Off")>=0)){
   EEPROM.write(5,0);
   delay(20);
   digitalWrite(port5,LOW);
   textMessage = "";
  sendSMS();
   
  }
  if((textMessage.indexOf("6ON")>=0)or(textMessage.indexOf("6on")>=0)or(textMessage.indexOf("6 On")>=0)){
   EEPROM.write(6,1);
   delay(20);
   digitalWrite(port6,HIGH);
    sendSMS();
  }
  if((textMessage.indexOf("6OFF")>=0)or(textMessage.indexOf("6off")>=0)or(textMessage.indexOf("6 Off")>=0)){
   EEPROM.write(6,0);
   delay(20);
   digitalWrite(port6,LOW);
   sendSMS();
  }
    if((textMessage.indexOf("ALLON")>=0)or(textMessage.indexOf("allon")>=0)or(textMessage.indexOf("Allon")>=0)){
   for(int i=1;i<7;i++){
   EEPROM.write(i,1); 
   delay(20);
   digitalWrite(i+1,HIGH);
  }
  sendSMS();
  
  }
   if((textMessage.indexOf("ALLOFF")>=0)or(textMessage.indexOf("alloff")>=0)or(textMessage.indexOf("Alloff")>=0)){
   for(int i=1;i<7;i++){
   EEPROM.write(i,0); 
   delay(20);
   digitalWrite(i+1,LOW);
  }
   sendSMS();
  
  }

  if((textMessage.indexOf("STATE")>=0)or(textMessage.indexOf("state")>=0)or(textMessage.indexOf("State")>=0)){
   
   delay(50);
   sendSMS();
   }

   
   if(textMessage.indexOf("AD")>=0){
   adminnum=textMessage.substring(textMessage.indexOf("AD 07"));
   adminnum=adminnum.substring(5);
   for(int i=0;i<8;i++){
    EEPROM.write(20+i,EEPROM.read(10+i));
    delay(20);
    temp=(adminnum.substring(i,1+i));
    num=temp.toInt();
    EEPROM.write(10+i,num);
    delay(20);
   }
   adminsms();
   
  }
  textMessage="";
  }
 delay(100);   
} 



// Function that sends SMS
void sendSMS(){
  digitalWrite(11,1);
  delay(100);
  digitalWrite(11,0);
  delay(200);
  digitalWrite(11,1);
  delay(100);
  digitalWrite(11,0); 
  for(int i=1;i<10;i++){
  mySerial.print("AT+CMGD=");
  mySerial.println(i);
  delay(50);
  }

 if(EEPROM.read(1)==1){ port1val="ON";}
 if(EEPROM.read(1)==0){port1val="OFF";} 
  if(EEPROM.read(2)==1){ port2val="ON";}
 if(EEPROM.read(2)==0){ port2val="OFF";}
  if(EEPROM.read(3)==1){ port3val="ON";}
 if(EEPROM.read(3)==0){ port3val="OFF";}
  if(EEPROM.read(4)==1){ port4val="ON";}
 if(EEPROM.read(4)==0){ port4val="OFF";}
  if(EEPROM.read(5)==1){ port5val="ON";}
 if(EEPROM.read(5)==0){ port5val="OFF";}
  if(EEPROM.read(6)==1){ port6val="ON";}
 if(EEPROM.read(6)==0){ port6val="OFF";}
  // AT command to set SIM900 to SMS mode
  mySerial.print("AT+CMGF=1\r"); 
  delay(100);

  
   mySerial.print("AT+CMGS=\"+947");
  for(int i =0;i<8;i++){
    mySerial.print(EEPROM.read(i+10));
    delay(50);
  }
    mySerial.println("\"");
  delay(50);
  mySerial.print("Port 1 is "); 
  delay(50);
  mySerial.print(port1val);
  delay(50);
  mySerial.print(".  Port 2 is "); 
  delay(50);
  mySerial.print(port2val);
  delay(50);
  mySerial.print(".  Port 3 is "); 
  delay(50);
  mySerial.print(port3val);
  delay(50);
  mySerial.print(".  Port 4 is "); 
  delay(50);
  mySerial.print(port4val);
  delay(50);
  mySerial.print(".  Port 5 is "); 
  delay(50);
  mySerial.print(port5val);
  delay(50);
  mySerial.print(".  Port 6 is "); 
  delay(50);
  mySerial.print(port6val);
  delay(50);
  mySerial.println("."); 
  delay(50);
                 
  mySerial.println((char)26); 
  delay(100);
  mySerial.println();
  delay(5000);  
}


void adminsms(){
   for(int i=1;i<10;i++){
  mySerial.print("AT+CMGD=");
  mySerial.println(i);
  delay(50);
  }
 mySerial.print("AT+CMGF=1\r"); 
  delay(100);

  
   mySerial.print("AT+CMGS=\"+947");
  for(int i =0;i<8;i++){
    mySerial.print(EEPROM.read(i+10));
    delay(50);
  }
    mySerial.println("\"");
  delay(50);  
  mySerial.print("Administrator number  is changed. New Number is +947 "); 
  delay(50);
  for(int i =0;i<8;i++){
    mySerial.print(EEPROM.read(i+10));
    delay(50);
  }
  mySerial.println("."); 
  delay(50);
                 
  mySerial.println((char)26); 
  delay(100);
  mySerial.println();
  delay(5000);
  //..................  
  mySerial.print("AT+CMGS=\"+947");
  for(int i =0;i<8;i++){
    mySerial.print(EEPROM.read(i+20));
    delay(50);
  }
    mySerial.println("\"");
  delay(50);  
  mySerial.print("Administrator number  is changed. New Number is +947 "); 
  delay(50);
  for(int i =0;i<8;i++){
    mySerial.print(EEPROM.read(i+10));
    delay(50);
  }
  mySerial.println("."); 
  delay(50);
                 
  mySerial.println((char)26); 
  delay(100);
  mySerial.println();
  delay(5000);  
}

