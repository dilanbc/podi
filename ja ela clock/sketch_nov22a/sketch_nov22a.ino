#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,2);
#define ringTime 5      ///////////////////////////////////
#define rl1 2
#define rl2 3
byte SHo,SMi,SSe;
#define timezones 16     ///////////////////////////////////////////////////
int school[timezones][2]=
{
{7,25},
{7,30},
{7,45},
{8,25},
{9,5},
{9,45},
{10,25},
{10,48},
{10,50},
{11,30},
{12,10},
{12,50},
{13,26},
{13,30},
{13,30},
{13,30},
};


#define mytimezones 4     ///////////////////////////////////////////////////
int myschool[mytimezones][2]=
{
{13,30},
{13,30},
{13,30},
{13,30},
};




void setup() {
  delay(500);
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  pinMode(rl1,1);
  pinMode(rl2,1);
  digitalWrite(rl1,1);
  digitalWrite(rl2,1);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  delay(200);

}

void loop() {
 tmElements_t tm;

  if (RTC.read(tm)) {
    SHo=tm.Hour;
    Serial.print(SHo);
    Serial.write(':');
    SMi=tm.Minute;
    Serial.print(SMi);
    Serial.write(':');
    SSe=tm.Second;
    Serial.print(SSe);
    //Serial.write('-');
    //print2digits(tm.Wday);
    
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
    }
    delay(9000);
  }
  delay(500);
 lcd.setCursor(4,0);
 char displayString[17];
 sprintf(displayString,"%02d:%02d:%02d",SHo,SMi,SSe);
 lcd.print(displayString);
 lcd.setCursor(3,1);
 sprintf(displayString,"%04d %02d %02d",tmYearToCalendar(tm.Year),tm.Month,tm.Day);
 lcd.print(displayString);
Timecheck();
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

bool Timecheck(){
  for(int i=0;i<timezones;i++){
    if((SHo==school[i][0])&&(SMi==school[i][1])&&(SSe<ringTime)){
       digitalWrite(rl1,0);
      break;
    }
    else{
       digitalWrite(rl1,1);
    }
    
  }
      
for(int i=0;i<mytimezones;i++){
    if((SHo==myschool[i][0])&&(SMi==myschool[i][1])&&(SSe<ringTime)){
       digitalWrite(rl2,0);
      break;
    }
    else{
       digitalWrite(rl2,1);
    }
    
  }
  
}
