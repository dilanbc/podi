#include <SoftwareSerial.h>
SoftwareSerial mySerial(8,9);
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include<EEPROM.h>
int myNum[9];            //this should be 9
char myword[15];
int v0;
int v1;
char v2;
char letter;
int port;
int enumber;
//......................................................................

void setup() {
  lcd.begin(16, 2);
    Serial.begin(115200);
    mySerial.begin(115200);
   epic();                                        //enable later
   entersetting();
   mainmenu:
   lcd.clear();

}
//...............................................................................................................................................
void loop(){
  lcd.setCursor(0,0);
  lcd.print("NO READINGS");

  if(analogRead(A1)>900){
    enumber=0;
    delay(250);
    sendsms();
    //call();
  }
  if(analogRead(A2)>900){
    enumber=40;
    delay(250);
    sendsms();
    //call();
  }
  if(analogRead(A3)>900){
    enumber=80;
    delay(250);
    sendsms();
    //call();
  }
  /*if(digitalRead(6)==1){
    enumber=120;
    delay(250);
    sendsms();
    //call();
  }
  if(digitalRead(7)==1){
    enumber=160;
    delay(250);
    sendsms();
    //call();
  }
  if(digitalRead(10)==1){
    enumber=200;
    delay(250);
    sendsms();
    //call();
  }
  */
}
//...............................................................................................................................................
void epic(){
  for(int x=14;x>=1;x--)
    {  
      lcd.setCursor(x, 0);  
       lcd.print("   WELCOME ");   
    lcd.setCursor(x, 1);  
       lcd.print("EPIC ENGINEERS  ");  
 delay (100);
    if (x==1)
    {
      delay(2000);
      }
    }
}
//.................................................
void entersetting(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press Red Button ");
  lcd.setCursor(0,1);
  lcd.print("to Enter Setting");
 for(int i=0;i<500;i++){
    delay(10);
    if(analogRead(A3)>900){
      
      delay(500);
      lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Button Pressed");

  entersetting:
          lcd.clear();

  delay(500);
  portn();
  delay(100);
  if(port==0){break;} 
 if (port ==1){enumber =0; }
 if (port ==2){enumber =40;}
 if (port ==3){enumber =80;}
 if (port ==4){enumber =120;}
 if (port ==5){enumber =160;}
 if (port ==6){enumber =200;}
 change:
 lcd.clear();
 lcd.setCursor(3,0);
 lcd.print("Port ");
 lcd.print(port);
 allread();
 lcd.setCursor(3,0);
 lcd.print("change ? ");
 lcd.setCursor(0,1);
 lcd.print("Number SMS Both");
 for(int i=0;i<5000;i++){
    delay(10);
    if(analogRead(A3)>900){savenumber(); goto entersetting;}
    if(analogRead(A2)>900){saveletter();goto entersetting;}
    if(analogRead(A1)>900){savenumber();saveletter(); goto entersetting;}
    if(analogRead(A4)>900){ goto entersetting; }
 }
 break;
 }
}
}
//.........................................
int portn(){
  do{lcd.setCursor(5,1);
  port = map(analogRead(A0),0,1023,1,6);
  lcd.print("Port ");
  lcd.print(port);
  delay(50);}while((analogRead(A3)<900)&&(analogRead(A4)<900));
  if(analogRead(A3)>900){return port;}
  if(analogRead(A4)>900){port=0; return port; }
  delay(100);
  
}
//..............................................
void allread(){
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("+94");
  for(int i  = 0; i<9;)
  {
    lcd.setCursor(i+3,0);
    lcd.print(EEPROM.read(i+enumber));
    delay(10);
    i=i+1;
  }
 lcd.setCursor(0,1);
   for(int i = 0; i<16;)
  {
    lcd.setCursor(i,1);
    Serial.print(v2);
    v2 = EEPROM.read(i+10 + enumber);
    delay(20);
    Serial.print(v2);

    lcd.print(v2);
    delay(10);
    i=i+1;
    
  }
  delay(5000);
  lcd.clear(); 
}
//.............................................................................................................................................

int savenumber(){              // USE FOR INSTALL NEW SETTING
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Rotate wheel & ");
  lcd.setCursor(3,1);
  lcd.print("Press Enter");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("+94");
  for( int i=0;i<9;){
    skip1:
    int v2=readpreset();
     if(v2==11){
      if(i==0){entersetting(); break;}
      delay(20);
      lcd.setCursor(i+2,0);
      lcd.print(" ");
      i=i-1;
      goto skip1;
      }
     myNum[i] = v2;
     EEPROM.write(i+enumber,v2);
     delay(20);
     lcd.setCursor(i+3,0);
     lcd.print(v2);
    delay(100);
   i=i+1;
   
    
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Number is = ");
  lcd.setCursor(0,1);
  lcd.print("+94");
  for(int i = 0; i<9;)
  {
    lcd.setCursor(i+3,1);
    lcd.print(myNum[i]);
    delay(10);
    i=i+1;
  }
 delay(5*1000);
}
//....................................
int readpreset(){
  do{
    v0 =map(analogRead(A0),0,1023,1,4);
  switch (v0){
    delay(100);
    case 1: lcd.setCursor(10,1);  lcd.print("  0  "); break;
    case 2:  lcd.setCursor(10,1); lcd.print("1 2 3"); break;
    case 3:  lcd.setCursor(10,1);lcd.print("4 5 6");break;
    case 4:  lcd.setCursor(10,1); lcd.print("7 8 9");break;
   
  }

    }while((analogRead(A3)<900)&&(analogRead(A2)<900)&&(analogRead(A1)<900)&&(analogRead(A4)<900));
   
   if(analogRead(A3)>900){
   switch(v0){
  case 1: v1=0; break;
  case 2: v1=1; break;
  case 3: v1=4; break;
  case 4: v1=7; break;
  }
   }
if(analogRead(A2)>900){
   switch(v0){
  case 1: v1=0; break;
  case 2: v1=2; break;
  case 3: v1=5; break;
  case 4: v1=8; break;
  }
}
if(analogRead(A1)>900){
   switch(v0){
  case 1: v1=0; break;
  case 2: v1=3; break;
  case 3: v1=6; break;
  case 4: v1=9; break;
  }
    
  }

if(analogRead(A4)>900){v1=11;}

 delay(250);
  return v1 ;
}
//.........................................................................................................
void saveletter()  // USE FOR INSTALL NEW Sentences
{            
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Rotate wheel & ");
  lcd.setCursor(3,1);
  lcd.print("Enter word");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
   for( int i=0;i<16;){
    skip2:
    lcd.setCursor(i,0);
    lcd.print("_");
    if(i<7){lcd.setCursor(0,1);  lcd.print(16-i); lcd.print("/16");}
    else{lcd.setCursor(0,1); lcd.print("0"); lcd.print(16-i); lcd.print("/16");}
     
    v2=englishletter1();
    if(v2==65){
      if(i==0){entersetting(); break;}
      delay(20);
      lcd.setCursor(i,0);
      lcd.print(" ");
      i=i-1;
      goto skip2;
      }
     myword[i] = v2;
     EEPROM.write(i+10 +enumber,v2);
     delay(20);
     lcd.setCursor(i,0);
     lcd.print(v2);
    delay(100);
   i=i+1;
    //Serial.println("number entered");
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sentences is ");
  lcd.setCursor(0,1);
  for(int i = 0; i<16;)
  {
    lcd.setCursor(i,1);
    lcd.print(myword[i]);
    delay(10);
    i=i+1;
  }
 delay(5*1000);
}
//....................................................................
int englishletter1(){
 
  do{
    v2 =map(analogRead(A0),0,1023,1,9);
  switch (v2){
    case 1: lcd.setCursor(10,1);  lcd.print("a b c"); break;
    case 2:  lcd.setCursor(10,1); lcd.print("d e f"); break;
    case 3:  lcd.setCursor(10,1);lcd.print("g h i");break;
    case 4:  lcd.setCursor(10,1); lcd.print("j k l");break;
    case 5:  lcd.setCursor(10,1);lcd.print("m n o");break;
    case 6:  lcd.setCursor(10,1); lcd.print("p q r");break;
    case 7:   lcd.setCursor(10,1);lcd.print("s t u");break;
    case 8:  lcd.setCursor(10,1); lcd.print("v w x");break;
    case 9:  lcd.setCursor(10,1); lcd.print("y z _ ");break;}
    delay(100);

    }while((analogRead(A3)<900)&&(analogRead(A2)<900)&&(analogRead(A1)<900)&&(analogRead(A4)<900));
if(analogRead(A3)>900){
   switch(v2){
  case 1: letter='a'; break;
  case 2: letter='d'; break;
  case 3: letter='g'; break;
  case 4: letter='j'; break;
  case 5: letter='m'; break;
  case 6: letter='p'; break;
  case 7: letter='s'; break;
  case 8: letter='v'; break;
  case 9: letter='y'; break; }
  }
if(analogRead(A2)>900){
   switch(v2){
  case 1: letter='b'; break;
  case 2: letter='e'; break;
  case 3: letter='h'; break;
  case 4: letter='k'; break;
  case 5: letter='n'; break;
  case 6: letter='q'; break;
  case 7: letter='t'; break;
  case 8: letter='w'; break;
  case 9: letter='z'; break; }
  }
if(analogRead(A1)>900){
   switch(v2){
  case 1: letter='c'; break;
  case 2: letter='f'; break;
  case 3: letter='i'; break;
  case 4: letter='l'; break;
  case 5: letter='o'; break;
  case 6: letter='r'; break;
  case 7: letter='u'; break;
  case 8: letter='x'; break;
  case 9: letter=' '; break; }
  } 
if(analogRead(A4)>900){letter ='A';}
 //Serial.println(letter);
  delay(100);
  return letter ;   
}
//................................................................................................................................................
void call(){
  lcd.clear();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calling . . .");
  lcd.setCursor(0,1);
  lcd.print("+94");
  for(int i = 0; i<9;)
  {
    lcd.setCursor(i+3,1);
    lcd.print(EEPROM.read(i+enumber));
    delay(10);
    i=i+1;
  }
  mySerial.print("ATD+94");
  for(int i =0;i<9;){
    mySerial.print(EEPROM.read(i+enumber));
    i=i+1;
  }
  mySerial.println(";\r");
  delay(15*1000);
  lcd.clear();
}
//............................................................
void sendsms()
{ 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sending sms . .");
  lcd.setCursor(0,1);
  lcd.print("+94");
  for(int i = 0; i<9;)
  {
    lcd.setCursor(i+3,1);
    lcd.print(EEPROM.read(i+enumber));
    delay(10);
    i=i+1;
  }

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  
  delay(50);
  
mySerial.print("AT+CMGS=\"+94");
  for(int i =0;i<9;){
    mySerial.print(EEPROM.read(i+enumber));
    i=i+1;
  }
  mySerial.println("\"");
  delay(50);
 
    delay(50);
for(int i = 0; i<16;)
  {
  
    v2 = EEPROM.read(i+10+enumber);
    delay(20);
  mySerial.print(v2);
    i=i+1;
  }
mySerial.println(".");
delay(50);

  mySerial.write(26);
 delay(15*1000);
 lcd.clear();
}
//.............................................................................................................................
