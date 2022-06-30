#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long time;
unsigned long time1;
unsigned long time2;
unsigned long time3;
float mpr=3200;
float diffrent;
float watt;
float pf;
float unit;
float total=0;
int count=0;
int temp=0;

byte LT[8] = 
{
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte UB[8] =
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte RT[8] =
{
  B11100,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte LL[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01111,
  B00111
};
byte LB[8] =
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};
byte LR[8] =
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100
};
byte UMB[8] =
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte LMB[8] =
{
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};


void setup() {
  lcd.createChar(8,LT);
  lcd.createChar(1,UB);
  lcd.createChar(2,RT);
  lcd.createChar(3,LL);
  lcd.createChar(4,LB);
  lcd.createChar(5,LR);
  lcd.createChar(6,UMB);
  lcd.createChar(7,LMB);
  lcd.begin(20,4);  
  welcome();
  lcd.setCursor(4,0);
 
  lcd.print("IMart Meter");
  Serial.begin(9600);
  lcd.setCursor(3,2);
  lcd.print("Waiting . . .");
  do{
      
    }while(analogRead(A0)<300);
    Serial.println(analogRead(A0));
   time = millis();
   time1=time;
   Serial.print("time1-");
   Serial.println(time1);
   delay(200);
   
}


void loop() {
 
   Serial.println( analogRead(A0));
   if(analogRead(A0)>300){
    lcd.setCursor(0,2);
    lcd.print("                    ");
    time = millis();
    time2=time;
    Serial.println(time2);
    Serial.print("diffrent-");
    diffrent=time2-time1;
    diffrent=diffrent/1000;
    Serial.println(diffrent);
    watt=(mpr*diffrent);  
    watt=(3600000/watt);
    Serial.print("watt-");
    Serial.println(watt);

    if(watt<=9){
    lcd.setCursor(3,2); 
    }
    else if(watt<=99){
    lcd.setCursor(2,2);  
    }
    else if(watt<=999){
    lcd.setCursor(1,2);   
    }
    else{
    lcd.setCursor(0,2);   
    }
    lcd.print(watt,0);
    lcd.setCursor(4,2);
    lcd.print("W");


    
    Serial.print("Power Factor");
    pf=(watt*diffrent/3600);
    Serial.println(pf);

    unit=((pf)/(1000*diffrent));
    Serial.print("unit-");
    Serial.println(unit,7);
    
    Serial.print("total-");
    total=total+(unit*diffrent);
    Serial.println(total,7);
    lcd.setCursor(2,1);
    lcd.print("0000000");
    if(total<=9){lcd.setCursor(8,1);}
    else if(total<=99){lcd.setCursor(7,1);}
    else if(total<=999){lcd.setCursor(6,1);}
    else if(total<=9999){lcd.setCursor(5,1);}
    else if(total<=99999){lcd.setCursor(4,1);}
    else if(total<=999999){lcd.setCursor(3,1);}
    else {lcd.setCursor(3,1);}
    lcd.print(total,5);
    lcd.setCursor(16,1);
    lcd.print("kWh");
    
    delay(5000);
    time1=time2;
   }
   
  time = millis();
  time3=time;
  time3=time3-time1;
  if((time3>180000)&&(temp==0)){
   Serial.print("stop");
   lcd.setCursor(0,2);
   lcd.print("                    "); 
   temp=1;   
 }
  delay(1);      
}


void welcome(){
  for(int i=0;i<7;i++){
  lcd.clear();  
  lcd.setCursor(i,1);
  lcd.write(1);
  lcd.write(255);
  lcd.write(1);
  lcd.setCursor(i,2);
  lcd.write(4);
  lcd.write(255);
  lcd.write(4); 

  lcd.setCursor(16-i,1);
  lcd.write(8);
  lcd.write(3);
  lcd.write(5);
  lcd.write(2);
  lcd.setCursor(16-i,2);
  lcd.write(255);
  lcd.write(254);
  lcd.write(254);
  lcd.write(255);
  delay(500);
  }

delay(5000);
lcd.clear();
  
}
/*void customI()
{
  lcd.setCursor(x,0);
  lcd.write(1);
  lcd.write(255);
  lcd.write(1);
  lcd.setCursor(x,1);
  lcd.write(4);
  lcd.write(255);
  lcd.write(4);
}
void customM()
{
  lcd.setCursor(x,0);
  lcd.write(8);
  lcd.write(3);
  lcd.write(5);
  lcd.write(2);
  lcd.setCursor(x,1);
  lcd.write(255);
  lcd.write(254);
  lcd.write(254);
  lcd.write(255);
}*/

