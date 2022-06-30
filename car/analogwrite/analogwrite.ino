#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 8); // RX, TX
const int rs = 13, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char letter;
void setup() {
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(6,OUTPUT);
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
  
}

void loop() { 
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
       lcd.setCursor(6,0);
       lcd.print("turn");
       lcd.setCursor(6,1);
       lcd.print("Left");
     }
if(letter=='d'){
      Serial.println("right");
       analogWrite(9,255);
       analogWrite(10,0);
       analogWrite(11,70);
       analogWrite(6,0);
       lcd.setCursor(6,0);
       lcd.print("Turn");
       lcd.setCursor(6,1);
       lcd.print("Right");
     }

if(letter=='w'){
      Serial.println("forward");
       analogWrite(9,255);
       analogWrite(10,0);
       analogWrite(11,255);
       analogWrite(6,0);
       lcd.setCursor(6,0);
       lcd.print("Run");
       lcd.setCursor(4,1);
       lcd.print("Forward");
     }
if(letter=='s'){
      Serial.println("back");
       analogWrite(9,0);
       analogWrite(10,100);
       analogWrite(11,0);
       analogWrite(6,100);
       lcd.setCursor(4,0);
       lcd.print("Reverse");
     }

if(letter=='p'){
      Serial.println("stop");
       analogWrite(9,0);
       analogWrite(10,0);
       analogWrite(11,0);
       analogWrite(6,0);
       lcd.setCursor(6,0);
       lcd.print("STOP");
     }


    
  }
  
}
