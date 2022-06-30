//..............................................................................LIBRARY......................................
#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9);
#include<EEPROM.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

//.............................................................................VARIABLES....................................
int myNum[9];
char myword[24];
int v0;
int v1;
char v2;
char letter;
int port;
int enumber;
int b1;
int b2;
int b3;  //for stop sound
int y;
int command;
int loopcount;
String textMessage;
int resetport;
int port1=1;  //d2
int port2=1;  //d10
int port3=1;  //d11
int port4=1;  //d12
int port5=1;  //d1
int port6=1;  //A5

//...........................................................................BITMAP...IMAGES................................
const unsigned char lefthand [] PROGMEM = {
  // 'lefthand, 42x32px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0,
  0x00, 0x00, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0f, 0xff,
  0xfc, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x80, 0x00, 0x00, 0x00,
  0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
};

const unsigned char righthand [] PROGMEM = {
  // 'righthand, 42x32px
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x7f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00,
  0x00, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x0f,
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00
};

const unsigned char setting [] PROGMEM = {
  // 'settting, 24x24px
  0x00, 0x78, 0x00, 0x00, 0x78, 0x00, 0x08, 0xfc, 0x40, 0x1f, 0xff, 0xe0, 0x3f, 0xff, 0xf0, 0x1f,
  0x87, 0xf0, 0x1e, 0x01, 0xe0, 0x1c, 0x00, 0xe0, 0x3c, 0x1c, 0xf0, 0xf8, 0x0e, 0x7c, 0xf8, 0x06,
  0x7c, 0xf8, 0x07, 0x7c, 0xf9, 0x8f, 0x7c, 0x3c, 0xff, 0x70, 0x1c, 0xff, 0xa0, 0x1e, 0x7f, 0xc0,
  0x1f, 0x87, 0xe0, 0x3f, 0xfb, 0xf0, 0x1f, 0xfd, 0xf8, 0x08, 0xfc, 0xfc, 0x00, 0x78, 0x7e, 0x00,
  0x78, 0x39, 0x00, 0x00, 0x19, 0x00, 0x00, 0x0e
};
const unsigned char phone [] PROGMEM = {
  // 'Untitled-3, 16x16px
  0x10, 0x00, 0x38, 0x00, 0x7c, 0x00, 0xfc, 0x00, 0xfc, 0x00, 0x78, 0x00, 0x70, 0x00, 0x30, 0x00,
  0x38, 0x00, 0x1c, 0x00, 0x1e, 0x1c, 0x0f, 0x3e, 0x07, 0xff, 0x01, 0xfe, 0x00, 0x7c, 0x00, 0x18
};
const unsigned char smslogo [] PROGMEM = {
  // 'smslogo, 16x16px
  0x00, 0x00, 0x7f, 0xfe, 0xc0, 0x03, 0xa0, 0x05, 0x90, 0x09, 0x88, 0x11, 0x84, 0x21, 0x86, 0x61,
  0x85, 0x81, 0x88, 0x11, 0x90, 0x09, 0xa0, 0x05, 0xc0, 0x03, 0xc0, 0x03, 0x7f, 0xfe, 0x00, 0x00
};
// 'back', 20x10px
const unsigned char back [] PROGMEM = {
  0x04, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x7f, 0xff, 0x00, 0x1c, 0x00, 0xc0, 0x04, 0x00, 0x60, 0x00, 
  0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x01, 0xc0, 0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00
};
// 'clear', 20x10px
const unsigned char clearicon [] PROGMEM = {
  0x07, 0xff, 0xf0, 0x0c, 0x00, 0x10, 0x38, 0x41, 0x10, 0x70, 0x77, 0x10, 0xc0, 0x1e, 0x10, 0xc0, 
  0x1c, 0x10, 0x60, 0x77, 0x10, 0x38, 0x41, 0x10, 0x0c, 0x00, 0x10, 0x07, 0xff, 0xf0
};
// 'clear', 20x10px
const unsigned char phonesmall [] PROGMEM = {
  // 'callsmall, 8x8px
  0x40, 0xe0, 0xe0, 0x40, 0x60, 0x36, 0x1f, 0x06
};
const unsigned char smssmall [] PROGMEM = {
  // 'smssmall, 8x8px
  0x00, 0xff, 0xc3, 0xa5, 0x99, 0x81, 0x81, 0xff
};
const unsigned char bad [] PROGMEM = {
  // 'bad, 8x8px
  0xc3, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0xc3
};

//....................................................................................................................................
void setup() {
  display.begin();
  Serial.begin(9600);
  mySerial.begin(9600);
  mainmenu:
  pinMode(13,OUTPUT);
  pinMode(12,INPUT);
  pinMode(11,INPUT);
  pinMode(10,INPUT);
  pinMode(12,INPUT);
  pinMode(1,INPUT);
  display.setContrast(57);
  display.clearDisplay();
  epic();           //enable later
  entersetting();
  display.clearDisplay();
}
//..........................................................................................................................................
void loop() {
  /*
  display.fillCircle(10, 40, 7, BLACK);
  display.fillCircle(10, 40, 4, WHITE);
  

  
  */
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("PORT1");
  if(port1==1){
   display.fillRect(30, 0, 8, 8,WHITE); 
  }
  if(port1==0){
   display.drawBitmap(30, 0,  bad, 8, 8, BLACK); 
  }
  display.setCursor(46,0);
  display.print("PORT2");
  if(port2==1){
   display.fillRect(76, 0, 8, 8,WHITE); 
  }
  if(port2==0){
   display.drawBitmap(76, 0,  bad, 8, 8, BLACK); 
  }
  display.setCursor(0,8);
  display.print("PORT3");
  if(port3==1){
   display.fillRect(30, 8, 8, 8,WHITE); 
  }
  if(port3==0){
   display.drawBitmap(30, 8,  bad, 8, 8, BLACK); 
  }
  display.setCursor(46,8);
  display.print("PORT4");
  if(port4==1){
   display.fillRect(76, 8, 8, 8,WHITE); 
  }
  if(port4==0){
   display.drawBitmap(76, 8,  bad, 8, 8, BLACK); 
  }
  display.setCursor(0,16);
  display.print("PORT5");
  if(port5==1){
   display.fillRect(30, 16, 8, 8,WHITE); 
  }
  if(port5==0){
   display.drawBitmap(30, 16,  bad, 8, 8, BLACK); 
  }
  display.setCursor(46,16);
  display.print("PORT6");
  if(port6==1){
   display.fillRect(76, 16, 8, 8,WHITE); 
  }
  if(port6==0){
   display.drawBitmap(76, 16,  bad, 8, 8, BLACK); 
  }
  display.fillCircle(10, 40, 7, BLACK);
  display.fillCircle(10, 40, 4, WHITE);
  display.display();

  


  
  if(analogRead(A3)>900){
    delay(100);
    b1=1;
    button();
    delay(500);
    portn();
    display.clearDisplay();
    }

  
  if(digitalRead(2)==1){  //port1
    delay(100);
    if(port1==0){
    }
    if(port1==1){
    display.display();   
    port1=0;
    enumber=0;
    delay(250);
    sendsms();       
    }
    }
    
 /*if(digitalRead(10)==1){  //port2
    delay(100);
    if(port2==0){
    }
    if(port2==1){
    port2=0;
    enumber=40;
    delay(250);
    sendsms();       
    }
    }
  */
 if(digitalRead(11)==1){  //port3
    delay(100);
    if(port3==0){
    }
    if(port3==1){
    port3=0;
    enumber=80;
    delay(250);
    sendsms();       
    }
    }
 if(digitalRead(12)==1){  //port4
    delay(100);
    if(port4==0){
    }
    if(port4==1){
    port4=0;
    enumber=120;
    delay(250);
    sendsms();       
    }
    }
 /*if(digitalRead(1)==1){  //port5
    delay(100);
    if(port5==0){
    }
    if(port5==1){
    port5=0;
    enumber=160;
    delay(250);
    sendsms();       
    }
    }*/
 if(analogRead(A5)>900){  //port6
    delay(100);
    if(port6==0){
    }
    if(port6==1){
    port6=0;
    enumber=200;
    delay(250);
    sendsms();       
    }
    }
                          
    if(mySerial.available()>0){
    //updateSerial();
    //Serial.println("  "); 
    textMessage = mySerial.readString();
    Serial.println(textMessage);    
    delay(10);
   
  if(textMessage.indexOf("RESET1")>=0){
    Serial.println("Port 1 Reset"); 
    port1=1;
    enumber=0; 
    textMessage = ""; 
    resetport=1;
    replymessage();
      
  }
  if(textMessage.indexOf("RESET2")>=0){
    port2=1;
    enumber=40;
    Serial.println("Port 2 Reset");  
    textMessage = ""; 
    resetport=2;
    replymessage();  
  }
  if(textMessage.indexOf("RESET3")>=0){
    port3=1;
    enumber=80;
    Serial.println("Port 3 Reset");  
    textMessage = "";
    resetport=3;
    replymessage();   
  }
  if(textMessage.indexOf("RESET4")>=0){
    port4=1;
    enumber=120;
    Serial.println("Port 4 Reset");  
    textMessage = "";
    resetport=4;
    replymessage();   
  }
  if(textMessage.indexOf("RESET5")>=0){
    port5=1;
    enumber=160;
    Serial.println("Port 5 Reset");  
    textMessage = "";
    resetport=5;
    replymessage();   
  }
  if(textMessage.indexOf("RESET6")>=0){
    port6=1;
    enumber=200;
    Serial.println("Port 6 Reset");  
    textMessage = "";
    resetport=6;
    replymessage(); 
  }
  }
  delay(100);
  
}
//.................................................................BUTTON...............................................................
void button() {
  
  
  digitalWrite(13,HIGH);
  delay(2);
  if (b3 == 1) {
    b3=0;
    digitalWrite(13,LOW);
   }
  if (b1 == 1) {
    display.fillCircle(10, 40, 2, BLACK);
    display.display();
    delay(100);
    display.fillCircle(10, 40, 2, WHITE);
    display.display();
  }
  if (b1 == 2) {
    display.fillCircle(30, 40, 2, BLACK);
    display.display();
    delay(100);
    display.fillCircle(30, 40, 2, WHITE);
    display.display();
  }
  if (b1 == 3) {
    display.fillCircle(50, 40, 2, BLACK);
    display.display();
    delay(100);
    display.fillCircle(50, 40, 2, WHITE);
    display.display();
  }
  if (b1 == 4) {
    display.fillRect(61, 35, 20, 10, WHITE);
    display.display();
    delay(100);
    display.drawBitmap(61, 35,  back, 20, 10, BLACK);
    display.display();
    delay(100);
   }
   if (b1 == 5) {
    display.fillRect(61, 38, 20, 10, WHITE);
    display.display();
    delay(100);
    display.drawBitmap(61, 38,  clearicon, 20, 10, WHITE);
    display.display();
   }
  b1 = 0;
  digitalWrite(13,LOW);
}
//................................................Update serial................................
void updateSerial(){
  delay(50);
 while(mySerial.available()) {
    Serial.write(mySerial.read());
  }
  while (Serial.available()) {
    mySerial.write(Serial.read());
  } 
}
//..................................................................EPIC......................................................
void epic() {
  for (int j = 0;  j < 22; j++) {
    display.clearDisplay();
    display.drawBitmap((2 * j) - 42, 21 - j,  lefthand, 42, 32, BLACK);
    display.drawBitmap(84 - 2 * j, 21 - j,  righthand, 42, 32, BLACK);
    display.display();
    delay(20);
  }

  for (int i = 0; i < 42; i++) {
    display.drawBitmap(0, 0,  lefthand, 42, 32, BLACK);
    display.drawBitmap(42, 0,  righthand, 42, 32, BLACK);
    
    display.setCursor(0, 32);
    display.print("EPIC ENGINEERS");
    display.fillRect(42+i, 32, 42, 8, WHITE);
    display.fillRect(-i-1, 32, 42, 8, WHITE);
    display.setCursor(0, 40);
    display.print("  (Pvt) Ltd.  ");
    display.fillRect(42+i, 40, 42, 8, WHITE);
    display.fillRect(-i, 40, 42, 8, WHITE);
    display.display();
    delay(10);
    }

   mySerial.println("AT");
   delay(50);
   //updateSerial(); 
   mySerial.println("AT+CMGF=1");
   delay(50);
   //updateSerial();
    for(int i=1;i<50;i++){
    mySerial.print("AT+CMGD=");
    mySerial.println(i);
    delay(50);
   //updateSerial();
   }
   mySerial.println("AT+CNMI=2,2,0,0,0");
   delay(50);
   //updateSerial();
   display.clearDisplay();
}
//.................................................................ENTER SETTING.................................................................
void entersetting() {
display.clearDisplay();
display.setCursor(0, 0);
display.print("   SETTINGS ");
display.fillCircle(10, 40, 7, BLACK);
display.fillCircle(10, 40, 4, WHITE);
display.drawBitmap(30, 12,  setting, 24, 24, BLACK);
display.drawBitmap(61, 35,  back, 20, 10, BLACK);

display.display();

for (int i = 0; i < 700; i++) {
    delay(10);
    int x = i % 50;
    if (x == 0) {
    
      y++;
      int z = y % 2;
      Serial.println(z);
      if (z == 0) {
       display.fillCircle(10, 40, 2, WHITE);
        display.display();
      }
      if (z == 1) {
        display.fillCircle(10, 40, 2, BLACK);
        display.display();
      }

    }
  if(analogRead(A3)>900){
    delay(100);
    portn();
    break;    
  }
  if(analogRead(A4)>900){
    delay(100);
    b1=4;
    button();
    display.clearDisplay();
    break;
  }
     
  }
}
//...............................................................PORT..................................................................
void portn() {
 display.clearDisplay();
 delay(100);
  do {
    display.setTextSize(1);
    display.setCursor(30, 0);
    display.print("Port ");
    display.fillCircle(10, 40, 7, BLACK);
    display.fillCircle(10, 40, 4, WHITE);
    display.drawBitmap(61, 35,  back, 20, 10, BLACK);
    port = map(analogRead(A0), 0, 1023, 1, 6);
    display.setCursor(35, 16);
    display.setTextSize(2);
    display.print(port);
    display.display();
    delay(50);
    display.fillRect(35, 16, 12, 16, WHITE);
  } while ((analogRead(A3) < 900) && (analogRead(A4) < 900));
   delay(50);
      if (port == 1) {
        enumber = 0;
      }
      if (port == 2) {
        enumber = 40;
      }
      if (port == 3) {
        enumber = 80;
      }
      if (port == 4) {
        enumber = 120;
      }
      if (port == 5) {
        enumber = 160;
      }
      if (port == 6) {
        enumber = 200;
      }

  
  if (analogRead(A3) > 900) {
     b1=1;
    button();
    allread();
      
  }
  if (analogRead(A4) > 900) {
   b1=4;
   button();
   display.clearDisplay();
   loop();
   
  }
}
//.................................................................ALL.READ............................................................
int allread() {
  display.clearDisplay();
  display.drawBitmap(61, 35,  back, 20, 10, BLACK);
  display.setTextSize(1);
  display.setCursor(3, 0);
  display.print("    Port ");
  display.print(port);
  display.drawBitmap(0, 10, phonesmall, 8, 8, BLACK);
  display.display();
  display.setCursor(14, 10);
  display.print("0");
  display.display();
  for (int i  = 0; i < 9;)
  {
    display.print(EEPROM.read(i + enumber));
    delay(10);
    i = i + 1;
    display.display();
  }
  display.drawBitmap(0, 18, smssmall, 8, 8, BLACK);
  display.display();
  display.setCursor(14, 18);
  display.display();
  for (int i = 0; i < 25;)
  {
    
    v2 = EEPROM.read(i + 10 + enumber);
    delay(20);
    display.print(v2);
    display.display();
    delay(10);
    i = i + 1;

  }
  display.fillCircle(10, 40, 7, BLACK);
  display.fillCircle(10, 40, 4, WHITE);
  display.display();
  do{
    delay(100);
    if(analogRead(A3)>900){
      delay(100);
      b1=1;
      button();
      display.clearDisplay();
      change();
      break;
    }

    if(analogRead(A4)>900){
     delay(100);
     b1=4;
     button();
     display.clearDisplay();
     portn();
     break; 
    }
    
  }while((analogRead(A3)<900)&&(analogRead(A4)<900));

}
//................................................................CHANGE...................................................................
void change(){
  start:
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(3, 5);
  display.print("change? ");
  display.setTextSize(1);
  display.setCursor(0, 24);
  display.print("Number SMS  ");
  display.fillCircle(10, 40, 7, BLACK);
  display.fillCircle(10, 40, 4, WHITE);
  display.fillCircle(50, 40, 7, BLACK);
  display.fillCircle(50, 40, 4, WHITE);
  display.drawBitmap(61, 35,  back, 20, 10, BLACK);
  display.display();
   do{
    delay(100);
    if(analogRead(A3)>900){
      delay(100);
      b1 = 1;
      button();
      display.clearDisplay();
      savenumber();
      if(command==1){
        goto start;
        break;
       }
      else{
        shownumber();
        goto start;
        break;
      }
     }

     if(analogRead(A1)>900){
     delay(100);
     b1=3;
     button();
     display.clearDisplay();
     saveletter();
    if(command==1){
        goto start;
        break;
       }
      else{
        showsms();
        goto start;
        break;
      }
    }

    if(analogRead(A4)>900){
     delay(100);
     b1=4;
     button();
     display.clearDisplay();
     allread();
     break; 
    }
    
  }while((analogRead(A3)<900)&&(analogRead(A3)<900)&&(analogRead(A4)<900));
}
//................................................................NUMBER............................................................
int savenumber() { 
  command=0;           
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Rotate wheel & Enter Number");
  display.fillCircle(10, 40, 7, BLACK);
  display.fillCircle(10, 40, 4, WHITE);
  display.fillCircle(30, 40, 7, BLACK);
  display.fillCircle(30, 40, 4, WHITE);
  display.fillCircle(50, 40, 7, BLACK);
  display.fillCircle(50, 40, 4, WHITE);
  display.display();
  for (int i = 0; i < 8; i++) {
    b3=1;
    b1 = i % 3;
    if (b1 == 0) {
      b1 = 3;
    }
    button();
  }
  display.clearDisplay();
  display.setCursor(0, 8);
  display.print("+94");
  display.fillCircle(10, 40, 7, BLACK);
  display.fillCircle(10, 40, 4, WHITE);
  display.fillCircle(30, 40, 7, BLACK);
  display.fillCircle(30, 40, 4, WHITE);
  display.fillCircle(50, 40, 7, BLACK);
  display.fillCircle(50, 40, 4, WHITE);
  display.display();

  for ( int i = 0; i < 9;) {
    skip1:
    if(i<1){
    display.fillRect(61, 36, 20, 10, WHITE);
    display.display();
    display.drawBitmap(61, 36,  back, 20, 10, BLACK);
    display.display();
    }
    if(i>0){
    display.fillRect(61, 36, 20, 10, WHITE);
    display.display();
    display.drawBitmap(61, 36,  clearicon, 20, 10, BLACK); 
    display.display();
    }
    int v2 = readpreset();
    display.setCursor(18 + 6 * i, 8);
    if (v2 == 11) {
      if (i == 0) {
        command=1;
        break;
        break;
      }
      delay(20);
      display.fillRect(12 + 6 * i, 8, 6, 8, WHITE);
      display.display();
      i = i - 1;
      goto skip1;
    }
    myNum[i] = v2;
    EEPROM.write(i + enumber, v2);
    delay(20);

    display.print(v2);
    display.display();
    delay(100);
    i = i + 1;


  }

}
//............................................................SHOW....NUMBER.......................................................
void shownumber(){
 
  display.clearDisplay();
  display.setCursor(0, 8);
  display.print("Number Saved");
  display.drawBitmap(0, 24, phone, 16, 16, BLACK);
  display.display();
  display.setCursor(18, 28);
  display.print("0");
  for (int i = 0; i < 9;)
  {
    
    display.print(myNum[i]);
    delay(10);
    display.display();
    i = i + 1;
  }
  delay(5 * 1000);
}
  
//....................................
int readpreset() {
  do {
    v0 = map(analogRead(A0), 0, 1023, 1, 4);
    switch (v0) {


      case 1: display.setCursor(3, 24); display.print("    0     "); display.display(); break;
      case 2: display.setCursor(3, 24); display.print(" 1  2  3"); display.display(); break;
      case 3: display.setCursor(3, 24); display.print(" 4  5  6"); display.display(); break;
      case 4: display.setCursor(3, 24); display.print(" 7  8  9"); display.display(); break;
    }
    delay(100);
    display.fillRect(0, 24, 84, 8, WHITE);
    display.display();
  } while ((analogRead(A3) < 900) && (analogRead(A2) < 900) && (analogRead(A1) < 900) && (analogRead(A4) < 900));

  if (analogRead(A3) > 900) {
    delay(100);
    b1=1;
    button();
    switch (v0) {
      case 1: v1 = 0; break;
      case 2: v1 = 1; break;
      case 3: v1 = 4; break;
      case 4: v1 = 7; break;
    }
  }
  if (analogRead(A2) > 900) {
    delay(100);
    b1=2;
    button();
    switch (v0) {
      case 1: v1 = 0; break;
      case 2: v1 = 2; break;
      case 3: v1 = 5; break;
      case 4: v1 = 8; break;
    }
  }
  if (analogRead(A1) > 900) {
    delay(100);
    b1=3;
    button();
    switch (v0) {
      case 1: v1 = 0; break;
      case 2: v1 = 3; break;
      case 3: v1 = 6; break;
      case 4: v1 = 9; break;
    }

  }

  if (analogRead(A4) > 900) {
    delay(100);
    b1=4;
    button();
    v1 = 11;
  }

  delay(250);
  return v1 ;
}
//.................................................................SAVE....WORD...............................................................
void saveletter(){
  command=0;  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Rotate wheel & Enter Number");
  display.fillCircle(10, 40, 7, BLACK);
  display.fillCircle(10, 40, 4, WHITE);
  display.fillCircle(30, 40, 7, BLACK);
  display.fillCircle(30, 40, 4, WHITE);
  display.fillCircle(50, 40, 7, BLACK);
  display.fillCircle(50, 40, 4, WHITE);
  display.display();
  for (int i = 0; i < 8; i++) {
    b3=1;
    b1 = i % 3;
    if (b1 == 0) {
      b1 = 3;
    }
    button();
  }
  display.fillRect(0, 0, 84, 16, WHITE);
  for ( int i = 0; i < 25;) {
     skip2:
     if(i<1){
    display.fillRect(61, 36, 20, 10, WHITE);
    display.display();
    display.drawBitmap(61, 36,  back, 20, 10, BLACK);
    display.display();
    }
    if(i>0){
    display.fillRect(61, 36, 20, 10, WHITE);
    display.display();
    display.drawBitmap(61, 36,  clearicon, 20, 10, BLACK); 
    display.display();}
    
    display.fillRect(50, 0,36, 8, WHITE);
    display.display();
    display.setCursor(50,0);
    display.print(25-i);
    display.print("/25");
    
    
    if(i<14){
    display.setCursor(6*i,8);
    display.print("_");
    display.display();
    v2 = englishletter1();
    if (v2 == 65) {
      if (i == 0) {
        command=1;
        break;
        }
      
      delay(20);
      display.fillRect(6*i-6, 8,12, 8, WHITE);
      display.display();
      i = i - 1;
      goto skip2;
      
    }
        
    display.fillRect(6*i, 8,6, 8, WHITE);
    display.setCursor(6*i, 8);
    display.print(v2);
    delay(100); 
     }
     else{
    display.setCursor(6*i-84,16);
    display.print("_");
    display.display();
    v2 = englishletter1();
    if (v2 == 65) {
      if (i == 14) {
       display.fillRect(78, 8,6, 8, WHITE); 
       display.display();
      }
      
     delay(20);
      display.fillRect(6*i-90, 16,12, 8, WHITE);
      display.display();
      i = i - 1;
      goto skip2;
      
    }
        
    display.fillRect(6*i-84, 16,6, 8, WHITE);
    display.setCursor(6*i-84, 16);
    display.print(v2);
    delay(100);
     }
 
    myword[i] = v2;
    EEPROM.write(i + 10 + enumber, v2);
    delay(20);
    i = i + 1;
   }
  
}
//............................................................................................................
void showsms(){ 
  display.clearDisplay();
  display.drawBitmap(34,0,smslogo,16,16,BLACK);
  display.setCursor(0, 24);
  display.display();
  for (int i = 0; i < 25;)
  {
    display.print(myword[i]);
    display.display();
    delay(10);
    i = i + 1;
  }
  delay(5 * 1000);
}
//..............................................................................................................................................
int englishletter1() {

  do {
    v2 = map(analogRead(A0), 0, 1023, 1, 9);
    switch (v2) {
      case 1: display.setCursor(3, 24); display.print(" a  b  c"); display.display(); break;
      case 2: display.setCursor(3, 24); display.print(" d  e  f"); display.display(); break;
      case 3: display.setCursor(3, 24); display.print(" g  h  i"); display.display(); break;
      case 4: display.setCursor(3, 24); display.print(" j  k  l"); display.display(); break;
      case 5: display.setCursor(3, 24); display.print(" m  n  o"); display.display(); break;
      case 6: display.setCursor(3, 24); display.print(" p  q  r"); display.display(); break;
      case 7: display.setCursor(3, 24); display.print(" s  t  u"); display.display(); break;
      case 8: display.setCursor(3, 24); display.print(" v  w  x"); display.display(); break;
      case 9: display.setCursor(3, 24); display.print(" y  z  _"); display.display(); break;
     }
    delay(100);
    display.fillRect(0, 24, 84, 8, WHITE);
    display.display();
  } while ((analogRead(A3) < 900) && (analogRead(A2) < 900) && (analogRead(A1) < 900) && (analogRead(A4) < 900));
  if (analogRead(A3) > 900) {
    delay(50);
    b1=1;
    button();
    switch (v2) {
      case 1: letter = 'a'; break;
      case 2: letter = 'd'; break;
      case 3: letter = 'g'; break;
      case 4: letter = 'j'; break;
      case 5: letter = 'm'; break;
      case 6: letter = 'p'; break;
      case 7: letter = 's'; break;
      case 8: letter = 'v'; break;
      case 9: letter = 'y'; break;
    } b1=1; button();   
  }
  if (analogRead(A2) > 900) {
    delay(50);
    b1=2;
    button();
    switch (v2) {
      case 1: letter = 'b'; break;
      case 2: letter = 'e'; break;
      case 3: letter = 'h'; break;
      case 4: letter = 'k'; break;
      case 5: letter = 'n'; break;
      case 6: letter = 'q'; break;
      case 7: letter = 't'; break;
      case 8: letter = 'w'; break;
      case 9: letter = 'z'; break;
    } b1=2; button();
  }
  if (analogRead(A1) > 900) {
    delay(50);
    b1=3;
    button();
    switch (v2) {
      case 1: letter = 'c'; break;
      case 2: letter = 'f'; break;
      case 3: letter = 'i'; break;
      case 4: letter = 'l'; break;
      case 5: letter = 'o'; break;
      case 6: letter = 'r'; break;
      case 7: letter = 'u'; break;
      case 8: letter = 'x'; break;
      case 9: letter = ' '; break;
    } b1=3; button();
  }
  if (analogRead(A4) > 900) {
    delay(50);
    b1=4;
    button();
    letter = 'A';
  }
  //Serial.println(letter);
  delay(100);
  return letter ;
}
void sendsms()
{ 
  display.clearDisplay();
  display.drawBitmap(34,0,smslogo,16,16,BLACK);
  display.setCursor(12,16);
  display.print("Sending SMS");
  display.setCursor(8,32);
  display.print("+94");
  display.display();
   for(int i = 0; i<9;)
  {
    display.print(EEPROM.read(i+enumber));
    display.display();
    delay(10);
    i=i+1;
  }

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  
  delay(50);
  
mySerial.print("AT+CMGS=\"+94");
  for(int i =0;i<9;){
    mySerial.print(EEPROM.read(i+enumber));
    i=i+1;
    delay(50);
  }
  mySerial.println("\"");
  delay(50);
 

for(int i = 0; i<25;)
  {
  
    v2 = EEPROM.read(i+10+enumber);
    delay(20);
  mySerial.print(v2);
    i=i+1;
  }
mySerial.print(".    Port ");
mySerial.print((enumber/40)+1);
mySerial.print(" is disabled. Reply 'RESET");
mySerial.print((enumber/40)+1);
mySerial.println("' to activate port");
delay(50);

  mySerial.write(26);
 delay(10*1000);
display.clearDisplay();
}
//..........................................................REPLY..SMS...................................................................
void replymessage(){
  
  mySerial.print("AT+CMGF=1\r"); 
  delay(100);
  
  mySerial.print("AT+CMGS=\"+94");
  for(int i =0;i<9;){
    mySerial.print(EEPROM.read(i+enumber));
    i=i+1;
    delay(50);
  }
  mySerial.println("\"");
  delay(50);
 
  mySerial.print("Port ");
  mySerial.print(resetport);
  mySerial.println(" reset .");
  mySerial.println((char)26); 
  delay(100);
  mySerial.println();
  delay(10*1000);  
}

//
