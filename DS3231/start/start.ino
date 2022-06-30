#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

//pin define
#define CS1 8
#define CS2 9
#define CS3 10

#define pushButton 2
#define ldr 3

bool pushEndState=0;
byte pushEndTime[2]={};  // minute Sec
volatile byte countTime[2]={};
volatile byte TimeNow[3]={};

void setup () 
{
  Serial.begin(9600);
  delay(1000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  rtc.adjust(DateTime(2020, 12, 14, 16, 16,30 ));      //date and time ajdust

  pinMode(CS1,OUTPUT);  //make bulb as output
  pinMode(CS2,OUTPUT);
  pinMode(CS3,OUTPUT);

  pinMode(pushButton,INPUT);
  digitalWrite(pushButton,HIGH);

  pinMode(ldr,INPUT);

  attachInterrupt(digitalPinToInterrupt(pushButton),pushTimeSet,RISING);
  //bulbControl(1,1,1);  //test bulb
  delay(100);
  bulbControl(0,0,0);
  pushEndTime[1]=0; pushEndTime[0]=0;
  pushEndState=0;
}

void loop () 
{
    DateTime now = rtc.now();
    Serial.print("Time- ");
    Serial.print(TimeNow[2]=now.hour());
    Serial.print(":");
    Serial.print(TimeNow[1]=now.minute());
    Serial.print(":");
    Serial.print(TimeNow[0]=now.second());

// countdown logic start
    if(pushEndState){
      if(TimeNow[0]>=pushEndTime[0]){
       countTime[0]=TimeNow[0]-pushEndTime[0];
        countTime[1]=TimeNow[1]-pushEndTime[1];
      }//end TimeNow[0]>=pushEndTime[0]
     else{
      countTime[0]=(TimeNow[0]+60)-pushEndTime[0];
      if((TimeNow[1]-1)>=pushEndTime[1]){
        countTime[1]=(TimeNow[1]-1)-pushEndTime[1];
      }
      else{
        countTime[1]=(TimeNow[1]+59)-pushEndTime[1];
      }
     }
     if(countTime[1]==20){
      pushEndState=0;
      countTime[1]=0;
      countTime[0]=0;
       pushEndTime[1]=0;
       pushEndTime[0]=0;
     }
    }//end pushEndState
// countdown logic end


    if(pushEndState){
      bulbControl(1,1,1);
    }
//ldr 
    else{
     if(digitalRead(ldr)){
       if((TimeNow[2]>=6)&&(TimeNow[2]<22)){// 6AM to 10PM
        bulbControl(1,1,1);
       }
        if((TimeNow[2]>=22)&&(TimeNow[2]<24)){// 10PM to 00AM
        bulbControl(0,1,1);
       }
       if ((TimeNow[2]>=0)&&(TimeNow[2]<6)){// 0AM to 6AM
        bulbControl(0,0,1);
       }
       
     }//end ldr
     else{
      bulbControl(0,0,0);
     }
    }
    
    Serial.print("  countdown- ");
    Serial.print(countTime[1]);
    Serial.print(":");
    Serial.print(countTime[0]);

    Serial.print("  State - ");
    Serial.print(pushEndState);

    Serial.print("  ldr - ");
    Serial.print(digitalRead(ldr));

    Serial.print("  bulb - ");
    Serial.print(digitalRead(CS1));
    Serial.print(digitalRead(CS2));
    Serial.println(digitalRead(CS3));

   
  
    delay(500);
}

void pushTimeSet(){
  pushEndState=1;
  pushEndTime[1]=TimeNow[1];
  pushEndTime[0]=TimeNow[0];
}

void bulbControl(bool BCS1,bool BCS2,bool BCS3){
  digitalWrite(CS1,BCS1);
  digitalWrite(CS2,BCS2);
  digitalWrite(CS3,BCS3);
}
