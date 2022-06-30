#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 8); // RX, TX
const int rs = 13, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char letter;

int red=A5;
int green=A4;
int blue=A3;

int trigPin = A0;    // Trigger
int echoPin = A1;    // Echo
unsigned long duration, cm,time1,time2;
int stoplength=20;


void setup() {
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  lcd.begin(16, 2);
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  Serial.println("Starting");
  mySerial.begin(9600);
   for(int x=14;x>=1;x--)
    {  
    lcd.setCursor(x, 1);  
       lcd.print("Rathnavali B.V ");  
 delay (100);
    if (x==1)
    {
      delay(500);
      }
    }
  for(int i=7;i>=0;i--){
  lcd.setCursor(0,0);
  lcd.print("  Bluetooth Car  ");
  Serial.println(i);
  for(int j=0;j<i;j++){
  lcd.setCursor(j,0);
  lcd.print(" "); 
  lcd.setCursor(15-j,0);
  lcd.print(" "); 
  Serial.println(j);  
  }
  delay(300);
  }
 lcd.clear(); 
}



void loop() { 

bluetooth();
look();


time1=millis();
do{
digitalWrite(trigPin, LOW);
delayMicroseconds(5);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
pinMode(echoPin, INPUT);
duration = pulseIn(echoPin, HIGH);
cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
Serial.print(cm);
Serial.print("cm");
Serial.println();
if(cm<stoplength){
lcd.clear();
analogWrite(9,0);
analogWrite(10,0);
analogWrite(11,0);
analogWrite(6,0);
lcd.setCursor(4,0);
lcd.print("      ");
lcd.setCursor(6,0);
lcd.print("STOP");
digitalWrite(red,HIGH);
digitalWrite(blue,LOW);
digitalWrite(green,HIGH);
delay(500);
do{
digitalWrite(trigPin, LOW);
delayMicroseconds(5);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
pinMode(echoPin, INPUT);
duration = pulseIn(echoPin, HIGH);
cm = (duration/2) / 29.1;
analogWrite(9,0);
analogWrite(10,100);
analogWrite(11,0);
analogWrite(6,100);
lcd.setCursor(4,0);
lcd.print("Reverse");
digitalWrite(red,HIGH);
digitalWrite(blue,HIGH);
digitalWrite(green,HIGH);    
}while(cm<stoplength+20);

lcd.setCursor(4,0);
lcd.print("       ");
Serial.println("stop");
analogWrite(9,0);
analogWrite(10,0);
analogWrite(11,0);
analogWrite(6,0);
lcd.setCursor(6,0);
lcd.print("STOP");
digitalWrite(red,LOW);
digitalWrite(blue,HIGH);
digitalWrite(green,HIGH);
}
 
}while(time1+500> millis());
  
  lcd.setCursor(6,1);
  lcd.print("    ");   
 
}

void look(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
   pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
   cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
 
 
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  lcd.setCursor(6,1);
  lcd.print(cm);
  if(cm<stoplength){
    lcd.clear();
   analogWrite(9,0);
   analogWrite(10,0);
   analogWrite(11,0);
   analogWrite(6,0);
   lcd.setCursor(6,0);
   lcd.print("STOP");
   digitalWrite(red,HIGH);
   digitalWrite(blue,LOW);
   digitalWrite(green,HIGH);
}
}  

 void bluetooth(){
  if (mySerial.available()) {
    lcd.clear();

    letter=mySerial.read();
    Serial.println(letter);
if(letter=='a'){
      Serial.println("left");
       analogWrite(9,120);
       analogWrite(10,0);
       analogWrite(11,255);
       analogWrite(6,0);
       lcd.setCursor(5,0);
       lcd.print("LEFT");
       digitalWrite(red,LOW);
       digitalWrite(blue,LOW);
       digitalWrite(green,HIGH);
       
        }
if(letter=='d'){
      Serial.println("right");
       analogWrite(9,255);
       analogWrite(10,0);
       analogWrite(11,70);
       analogWrite(6,0);
       lcd.setCursor(5,0);
       lcd.print("RIGHT");

       digitalWrite(red,HIGH);
       digitalWrite(blue,LOW);
       digitalWrite(green,LOW);
     }

if(letter=='w'){
      Serial.println("forward");
       analogWrite(9,255);
       analogWrite(10,0);
       analogWrite(11,255);
       analogWrite(4,0);
       lcd.setCursor(4 ,0);
       lcd.print("FORWARD");
       digitalWrite(red,LOW);
       digitalWrite(blue,HIGH);
       digitalWrite(green,LOW);
     }
if(letter=='s'){
      Serial.println("back");
       analogWrite(9,0);
       analogWrite(10,100);
       analogWrite(11,0);
       analogWrite(6,100);
       lcd.setCursor(4,0);
       lcd.print("Reverse");
       digitalWrite(red,HIGH);
       digitalWrite(blue,HIGH);
       digitalWrite(green,HIGH);
     }

if(letter=='p'){
      Serial.println("stop");
       analogWrite(9,0);
       analogWrite(10,0);
       analogWrite(11,0);
       analogWrite(6,0);
       lcd.setCursor(6,0);
       lcd.print("STOP");
       digitalWrite(red,LOW);
       digitalWrite(blue,HIGH);
       digitalWrite(green,HIGH);
     }


    
  }

}
