#include <EEPROM.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int light=7;
int state=8;
int dislike=9;
int like=10;
int temp1=1;




byte three[8] = {
  0b11100,
0b11100,
0b11100,
0b11100,
0b11100,
0b11100,
0b11100,
0b11100
};



byte five[8] = {
  0b11111,
0b11111,
0b11111,
0b11111,
0b11111,
0b11111,
0b11111,
0b11111
};
byte buttonchar[8] = {
  0b00000,
  0b01110,
  0b11111,
  0b11011,
  0b11111,
  0b01110,
  0b00000,
  0b00000
};
byte rightchar[8] = {
  0b00000,
  0b00000,
  0b00001,
  0b00010,
  0b10100,
  0b01000,
  0b00000,
  0b00000
};
byte likechar[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

byte dislikechar[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b01110,
  0b10001,
  0b00000,
  0b00000
};
byte zerochar[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};





void setup() {
Serial.begin(9600);
Serial.println("Started");
lcd.begin();
lcd.createChar(3,three);
lcd.createChar(5,five);
lcd.createChar(6,buttonchar);
lcd.createChar(7,rightchar);
lcd.createChar(8,likechar);
lcd.createChar(9,dislikechar);
lcd.createChar(10,zerochar);
pinMode(light,OUTPUT);
pinMode(state,INPUT);
pinMode(dislike,INPUT);
pinMode(like,INPUT);
digitalWrite(light,HIGH);
welcome();
vote();
}

void welcome(){
  
for(int x=14;x>=1;x--)
    {  
      lcd.setCursor(x, 0);  
       lcd.print("Feedback System");   
    lcd.setCursor(x, 1);  
       lcd.print("  IET-Mech16  ");  
 delay (100);
    if (x==1)
    {
      delay(2000);
      }
}
lcd.clear();
}

void loop() {

do{
  vote();
  temp1++;
  } while((digitalRead(like)==0)&&(digitalRead(dislike)==0)&&(digitalRead(state)==0));
lcd.clear();
if(digitalRead(like)==1){
  delay(100);
  if(EEPROM.read(1)==255){
     EEPROM.write(1,0);
     EEPROM.write(2,EEPROM.read(2)+1);
    }
   if(EEPROM.read(1)<255){
    EEPROM.write(1,EEPROM.read(1)+1); 
    delay(250);
    }
    lcd.clear();
    lcd.setCursor(7,0);
    lcd.write(8);
    lcd.setCursor(5,1);
    lcd.print("Liked");
    lcd.setCursor(10,1);
    lcd.write(7);
    delay(1000);
    lcd.clear();
    readdata();
 }
 if(digitalRead(dislike)==1){
   delay(100);
  if(EEPROM.read(3)==255){
     EEPROM.write(3,0);
     EEPROM.write(4,EEPROM.read(4)+1);
    }
   if(EEPROM.read(1)<255){
    EEPROM.write(3,EEPROM.read(3)+1); 
    delay(250);
    }
    lcd.clear();
    lcd.setCursor(7,0);
    lcd.write(9);
    lcd.setCursor(4,1);
    lcd.print("Disliked");
    lcd.setCursor(12,1);
    lcd.write(7);
    delay(1000);
    lcd.clear();
    readdata();
 }
 if(digitalRead(state)==1){
  delay(100);
  readdata();
  if(digitalRead(state)==1){
   lcd.clear();
   lcd.setCursor(1,0);
   lcd.print("Ready to clear");
   for(int i=0;i<16;i++){
    lcd.setCursor(i,1);
    lcd.write(5);
    if(digitalRead(state)==0){
      lcd.clear();
      goto skip;
     }
     delay(700);
   }   
  resetdata();
  }
 skip:;
 }
delay(100);
}

void vote(){
lcd.setCursor(0,0);
lcd.print(" How's the food?");
lcd.setCursor(4,1);
lcd.write(8); 
lcd.setCursor(10,1);
lcd.write(9);

if(temp1%100==50){
lcd.setCursor(5,1);
lcd.write(6);
lcd.setCursor(11,1);
lcd.write(10);
}
if(temp1%100==0){
lcd.setCursor(5,1);
lcd.write(10);
lcd.setCursor(11,1);
lcd.write(6);
temp1=1; 
}
delay(10);
}

void readdata(){
 lcd.clear();
 int vlike=((EEPROM.read(2)*255)+(EEPROM.read(1))); 
 int vdislike=((EEPROM.read(4)*255)+(EEPROM.read(3))); 
 int plike=(vlike*100)/(vlike+vdislike);
 int pdislike=(vdislike*100)/(vlike+vdislike);
 int temp2;
 for(int i=0;i<10;i++){
  lcd.setCursor(i,0);
  if(i<(plike/10)){
    lcd.write(5);
  }
  if(i==(plike/10)){
   switch((plike%10)/2){
    case 0: lcd.write(10); break;
    case 1: lcd.write(10); break;
    case 2: lcd.write(10); break;
    case 3: lcd.write(3); break;
    case 4: lcd.write(5); break;
    }
  }
  lcd.setCursor(i,1);
   if(i<(pdislike/10)){
    lcd.write(5);
  }
  if(i==(pdislike/10)){
  switch((pdislike%10)/2){
    case 0: lcd.write(10); break;
    case 1: lcd.write(10); break;
    case 2: lcd.write(10); break;
    case 3: lcd.write(3); break;
    case 4: lcd.write(5); break;
    }
  }
 delay(50); 
 }
 lcd.setCursor(10,0);
 lcd.print(plike);
 lcd.setCursor(10,1);
 lcd.print(pdislike);
 lcd.setCursor(13,0);
 lcd.print(vlike);
 lcd.setCursor(13,1);
 lcd.print(vdislike);
 delay(5000);
 lcd.clear();
}

void resetdata(){
 EEPROM.write(1,0); 
 EEPROM.write(2,0); 
 EEPROM.write(3,0); 
 EEPROM.write(4,0); 
 lcd.clear();
 lcd.setCursor(1,0);
 lcd.print("Memory Cleared");
 delay(3000);
}

