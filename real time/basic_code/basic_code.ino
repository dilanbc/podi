#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

int ontime=1815;
int offtime=530;
int timenow;
int red=8;
int blue=9;
void setup () 
{
 // Serial.begin(9600);
  delay(1000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

pinMode(red,OUTPUT);
pinMode(blue,OUTPUT);
digitalWrite(red,LOW);
digitalWrite(blue,HIGH);
}

void loop () 
{
    DateTime now = rtc.now();
    int timenow=((now.hour())*100)+(now.minute());
    Serial.println(timenow);

   if((ontime<=timenow)&&(timenow<=2359)){
    digitalWrite(blue,HIGH);
    digitalWrite(red,LOW);
    Serial.println("in");
   }
   if((0<=timenow)&&(timenow<=offtime)){
    digitalWrite(blue,HIGH);
    digitalWrite(blue,LOW);
    Serial.println("in");
   }
   
   if((offtime<timenow)&&(timenow<ontime)){
   digitalWrite(blue,LOW); 
   digitalWrite(red,HIGH);
   Serial.println("out");
   }
    
    //Serial.println(digitalRead(LED));

    delay(1000);
}
