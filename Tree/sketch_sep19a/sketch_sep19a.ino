#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11

#define chumup 6
#define chumdown 7

#define ctempdown 5
float hum;
float temp;
DHT dht(DHTPIN, DHTTYPE);

int BH1750address = 0x23; //i2c address
bool switch1=0;
unsigned int lux=0;
 
byte buff[2];
void setup()
{
  Wire.begin();
  dht.begin();
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  pinMode(chumup,1);
  pinMode(chumdown,1);
  pinMode(ctempdown,1);
  Serial.begin(9600);
  BH1750_Init(BH1750address);
  lcd.setCursor(4,0);
  lcd.print("Welcome");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" LUX  HUM  TEMP");
}
 
void loop()
{
  switch1=!switch1;
  if(switch1==0){
    
 //lux reading 
  if(2==BH1750_Read(BH1750address))
  {
    lux=((buff[0]<<8)|buff[1])/1.2;
    Serial.print(lux,DEC);     
    Serial.print("lux    "); 
  }

// CO reading


//Humidity
 hum=dht.readHumidity();
Serial.print("  Humidity- ");
Serial.print(hum);

//Temp
  temp = dht.readTemperature();
  Serial.print("  temp- ");
  Serial.println(temp);

 //lcd
 lcd.setCursor(0,1);
 lcd.print("    ");
 if (lux<1000){
  lcd.setCursor(1,1); 
 }
 else{lcd.setCursor(0,1); }
 lcd.print(lux);

 lcd.setCursor(5,1);
 lcd.print("   ");
 lcd.setCursor(5,1);
 lcd.print(hum,1);
 lcd.setCursor(9,1);
 lcd.print("%");

 lcd.setCursor(11,1);
 lcd.print("   ");
 lcd.setCursor(11,1);
 lcd.print(temp,1);
 lcd.print("C");

  }
  else{
  float chum=analogRead(A0);
  chum=(chum/1023)*100;
  lcd.setCursor(5,1);
 lcd.print("      ");
 lcd.setCursor(6,1);
 lcd.print(chum,0);
if(chum>hum){
  digitalWrite(chumdown,1);
  digitalWrite(chumup,0);
}
else{digitalWrite(chumdown,0);
  digitalWrite(chumup,1);}

 /////////////////////////////
 float ctemp=analogRead(A1);
 ctemp=((ctemp/1023)*35)+15;
 lcd.setCursor(11,1);
 lcd.print("       ");
 lcd.setCursor(12,1);
 lcd.print(ctemp,0);
if(ctemp>temp){
  digitalWrite(ctempdown,1);
}
else{digitalWrite(ctempdown,0);}

  
  
  }

 
  
  delay(1000);
}


 
int BH1750_Read(int address) //
{
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()) //
  {
    buff[i] = Wire.read();  // receive one byte
    i++;
  }
  Wire.endTransmission();  
  return i;
}
 
void BH1750_Init(int address) 
{
  Wire.beginTransmission(address);
  Wire.write(0x10);//1lx reolution 120ms
  Wire.endTransmission();
}
