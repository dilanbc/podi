//..............................................................................LIBRARY......................................
#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9);
#include<EEPROM.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//.............................................................................VARIABLES....................................
int myNum[9];
char myword[29];
int v0;
int v1;
char v2;
char letter;
int port;
int enumber;
int b1;
int b2;
int y;
int command;
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
//....................................................................................................................................
void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  display.begin();
  Serial.begin(115200);
  mySerial.begin(115200);
  mainmenu:
  display.setContrast(57);
  display.clearDisplay();
  //epic();           //enable later
  entersetting();
  display.clearDisplay();
}
//..........................................................................................................................................
void loop() {
  
  display.setTextSize(1);
  display.setCursor(0,8);
  display.print("NO READINGS");
  display.display();
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
  delay(100);
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
  }*/

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

  for (int i = 0; i < 83; i++) {
    display.drawBitmap(0, 0,  lefthand, 42, 32, BLACK);
    display.drawBitmap(42, 0,  righthand, 42, 32, BLACK);
    display.setCursor(i - 82, 32);
    display.print("EPIC ENGINEERS");
    display.setCursor(84 - i, 40);
    display.print("  (Pvt) Ltd.  ");
    display.display();
    delay(10);
    display.clearDisplay();
  }

  delay(3000);
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
  display.display();

  for (int i = 0; i < 700; i++) {
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
    delay(10);

    if (analogRead(A3) > 900) {
      delay(500);
      display.clearDisplay();
entersetting:
      portn();
      delay(50);
      if (port == 0) {
        break;
      }
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
change:
      display.clearDisplay();
      allread();
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
      display.fillCircle(76, 40, 7, BLACK);
      display.fillCircle(76, 40, 4, WHITE);
      display.display();
      for (int i = 0; i < 5000; i++) {
        delay(10);
        if (analogRead(A3) > 900) {
          delay(200);
          b1 = 1;
          button();
          savenumber();
          goto entersetting;
        }
        if (analogRead(A1) > 900) {
          delay(200);
          b1 = 3;
          button();
          saveletter();
          if(command==1){
            command=0;
            break;
          }
          else{
          showsms();
          allread();
          break;  
          }
          
          
        }
        if (analogRead(A4) > 900) {
          delay(200);
          b1 = 4;
          button();
          goto entersetting;
        }
      }
      break;
    }
  }
}

//.................................................................BUTTON...............................................................
void button() {
  if (b1 == 1) {
    display.fillCircle(10, 40, 2, BLACK);
    display.display();
    delay(250);
    display.fillCircle(10, 40, 2, WHITE);
    display.display();
  }
  if (b1 == 2) {
    display.fillCircle(30, 40, 2, BLACK);
    display.display();
    delay(250);
    display.fillCircle(30, 40, 2, WHITE);
    display.display();
  }
  if (b1 == 3) {
    display.fillCircle(50, 40, 2, BLACK);
    delay(250);
    display.fillCircle(50, 40, 2, WHITE);
    display.display();
  }
  if (b1 == 4) {
    display.fillCircle(76, 40, 2, BLACK);
    display.display();
    delay(250);
    display.fillCircle(76, 40, 2, WHITE);
    display.display();
  }
  b1 = 0;
}
//.................................................................PORT....................................................................
int portn() {
  do {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(30, 0);
    display.print("Port ");
    display.fillCircle(10, 40, 7, BLACK);
    display.fillCircle(10, 40, 4, WHITE);
    display.fillCircle(76, 40, 7, BLACK);
    display.fillCircle(76, 40, 4, WHITE);
    port = map(analogRead(A0), 0, 1023, 1, 6);
    display.setCursor(35, 16);
    display.setTextSize(2);
    display.print(port);
    display.display();
    delay(50);
  } while ((analogRead(A3) < 900) && (analogRead(A4) < 900));
  if (analogRead(A3) > 900) {
    b1 = 1;
    button();
    return port;
  }
  if (analogRead(A4) > 900) {
    b1 = 4;
    button();
    port = 0;
    return port;
  }

}
//.................................................................ALL.READ............................................................
int allread() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(3, 0);
  display.print("    Port ");
  display.print(port);
  display.drawBitmap(0, 8, phone, 16, 16, BLACK);
  display.display();
  display.setCursor(20, 12);
  display.print("0");
  display.display();
  for (int i  = 0; i < 9;)
  {
    display.print(EEPROM.read(i + enumber));
    delay(10);
    i = i + 1;
    display.display();
  }
  display.drawBitmap(0, 28, smslogo, 16, 16, BLACK);
  display.display();
  display.setCursor(20, 28);
  display.display();
  for (int i = 0; i < 16;)
  {
    if (i == 10) {
      display.setCursor(20, 40);
    }
    v2 = EEPROM.read(i + 10 + enumber);
    delay(20);
    display.print(v2);
    display.display();
    delay(10);
    i = i + 1;

  }
  do{
    delay(100);
    if(analogRead(A3)>900){
      
    }

    if(analogRead(A4)>900){
      
    }
    
  }while((analogRead(A3)<900)&&(analogRead(A4)<900))


  
  display.clearDisplay();
}
//................................................................NUMBER............................................................
int savenumber() {            
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
    int v2 = readpreset();
    display.setCursor(18 + 6 * i, 8);
    if (v2 == 11) {
      if (i == 0) {
        entersetting();
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
    switch (v0) {
      case 1: v1 = 0; break;
      case 2: v1 = 1; break;
      case 3: v1 = 4; break;
      case 4: v1 = 7; break;
    }
  }
  if (analogRead(A2) > 900) {
    switch (v0) {
      case 1: v1 = 0; break;
      case 2: v1 = 2; break;
      case 3: v1 = 5; break;
      case 4: v1 = 8; break;
    }
  }
  if (analogRead(A1) > 900) {
    switch (v0) {
      case 1: v1 = 0; break;
      case 2: v1 = 3; break;
      case 3: v1 = 6; break;
      case 4: v1 = 9; break;
    }

  }

  if (analogRead(A4) > 900) {
    v1 = 11;
  }

  delay(250);
  return v1 ;
}
//.................................................................SAVE....WORD...............................................................
void saveletter()  
{ display.clearDisplay();
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
    b1 = i % 3;
    if (b1 == 0) {
      b1 = 3;
    }
    button();
  }
  display.fillRect(0, 0, 84, 16, WHITE);
  for ( int i = 0; i < 25;) {
    
skip2:
    display.fillRect(50, 0,36, 8, WHITE);
    display.display();
    display.setCursor(50,0);
    display.print(30-i);
    display.print("/30");
    
    
    if(i<14){
    display.setCursor(6*i,8);
    display.print("_");
    display.display();
    v2 = englishletter1();
    if (v2 == 65) {
      if (i == 0) {
        allread();
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
  for (int i = 0; i < 16;)
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
    letter = 'A';
  }
  //Serial.println(letter);
  delay(100);
  return letter ;
}
//................................................................................................................................................
void call() {
  lcd.clear();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calling . . .");
  lcd.setCursor(0, 1);
  lcd.print("+94");
  for (int i = 0; i < 9;)
  {
    lcd.setCursor(i + 3, 1);
    lcd.print(EEPROM.read(i + enumber));
    delay(10);
    i = i + 1;
  }
  mySerial.print("ATD+94");
  for (int i = 0; i < 9;) {
    mySerial.print(EEPROM.read(i + enumber));
    i = i + 1;
  }
  mySerial.println(";\r");
  delay(15 * 1000);
  lcd.clear();
}
//..............................................................................................................................................
void sendsms()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Sending sms..");
  display.setCursor(20,8);
  display.print("0");
  display.display();
  for (int i = 0; i < 9;)
  {
    
    display.print(EEPROM.read(i + enumber));
    display.display();
    delay(10);
    i = i + 1;
  }

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode

  delay(50);

  mySerial.print("AT+CMGS=\"+94");
  for (int i = 0; i < 9;) {
    mySerial.print(EEPROM.read(i + enumber));
    i = i + 1;
  }
  mySerial.println("\"");
  delay(50);

  delay(50);
  display.setCursor(8,24);
  for (int i = 0; i < 30;)
  {

    v2 = EEPROM.read(i + 10 + enumber);
    display.print(v2);
    display.display();
    delay(20);
    mySerial.print(v2);
    i = i + 1;
  }
  mySerial.println(".");
  delay(50);

  mySerial.write(26);
  delay(15 * 1000);
  display.clearDisplay();
}
//.............................................................................................................................
