#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

int timeNow;
int onTime;
int offTime;

const int set[15][5]=
{
  {1,800,1632}, //[0] 
  {2,1800,430}, //[1]
  {3,700,800,1700,1800}, //[2]
  {4,600,1800,2200,300}, //[3]
  {}, //[5]
  {},
  {},
  {},
  {},
  {},
  {},
  {},
  {},
  {},
  {},
 };


byte pattonNum;

int on=8;
int off=9;

void setup () 
{
  Serial.begin(9600);
  delay(500); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
//rtc.adjust(DateTime(2019, 11, 30, 2, 55, 0));      //remove
pinMode(10,INPUT);
pinMode(11,INPUT);
pinMode(12,INPUT);
pinMode(13,INPUT);
pinMode(on,OUTPUT);
pinMode(off,OUTPUT);
}

void loop () 
{   
    pattonNum=readpatton();
    Serial.println(pattonNum);
    
    DateTime now = rtc.now();
    timeNow=((now.hour())*100)+(now.minute());
    Serial.println(timeNow);
    if(thinkPart()==1){
     Serial.println("ON");
     digitalWrite(on,HIGH); 
     digitalWrite(off,LOW);
    }
    else{
    Serial.println("OFF");
    digitalWrite(on,LOW);  
    digitalWrite(off,HIGH); 
    }
    
delay(100);
}



byte readpatton(){
pattonNum=(digitalRead(10)*8)+(digitalRead(11)*4)+(digitalRead(12)*2)+(digitalRead(13)*1);
return pattonNum;

}

bool thinkPart(){
 bool state;
 //
 if(set[pattonNum][0]==0){
  state=0;
 }
 //
 
 if(set[pattonNum][0]==1){
  if((set[pattonNum][1]<timeNow)&&(timeNow<set[pattonNum][2])){
    state=1; 
  }
  else{
   state=0; 
  }
 }
//
 
 if(set[pattonNum][0]==2){
 if((set[pattonNum][2]<timeNow)&&(timeNow<set[pattonNum][1])){
  state=0;  
 }
 else{
 state=1;
 }
 }
 //
 if(set[pattonNum][0]==3){
  if(((set[pattonNum][1]<timeNow)&&(timeNow<set[pattonNum][2]))||((set[pattonNum][3]<timeNow)&&(timeNow<set[pattonNum][4]))){
   state=1;  
  }
  else{
   state=0; 
  }
 }
 if(set[pattonNum][0]==4){
  if((set[pattonNum][4]<timeNow)&&(timeNow<set[pattonNum][3])){
   if((set[pattonNum][1]<timeNow)&&(timeNow<set[pattonNum][2])){
   state=1;  
   }
   else{
   state=0; 
   }
  }
  else{
   state=1; 
  }
 }
 return state;
}
