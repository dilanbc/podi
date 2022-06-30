#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(9, 8);            // RX, TX Pins
String apn = "dialogbb";                       //APN
String apn_u = "";                     //APN-Username
String apn_p = "";                     //APN-Password
String url = "http://imart.a4tours.net/updtMtr.php";  //URL for HTTP-POST-REQUEST
String data1;   //String for the first Paramter (e.g. Sensor1)
String data2;   //String for the second Paramter (e.g. Sensor2)

const int rs = 11, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long time;
unsigned long time1;
unsigned long time2;
unsigned long time3;
unsigned long server1=0;
unsigned long server2;
unsigned long wait1=0;
unsigned long wait2;
float mpr=3200;
float diffrent;
float watt;
float pf;
float unit;
float total=0;
int count=0;
int temp=0;



int bulb;
int i=0;
int j;
int count1 =0;

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
  pinMode(7,OUTPUT);     // power control
  digitalWrite(7,EEPROM.read(0));
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);
  delay(1000);
  digitalWrite(10,LOW);  
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

  mySerial.begin(9600);
 // delay(5000);
   
}


void loop() {
   time = millis();
    server2=time;

 
   Serial.println( analogRead(A0));
   if(analogRead(A0)>300){
     pulsecount();
 }

 if((server2-server1)>20000){      //server update delay// change this.
    server1=server2;  
    data1 = "";
    data2 = String(total,5);
    gsm_sendhttp(); //Start the GSM-Modul and start the transmisson
    

////////////////////////////////////////
 }

time = millis();
  time3=time;
  time3=time3-time1;
  if((time3>180000)&&(temp==0)){  // for keep last value for few second
   Serial.print("stop");
   lcd.setCursor(0,2);
   lcd.print("                    "); 
   temp=1;
 
  delay(1);      
}
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


void gsm_sendhttp() {
  
  
 
  mySerial.println("AT");
  runsl();//Print GSM Status an the Serial Output;
  time = millis();
  wait1=time;
  do{
    delay(1);//////////////////////////////////////////
  time = millis();
  wait2=time;
  Serial.println(analogRead(A0));
  if(analogRead(A0)>300){
     pulsecount();
  }
  
  }while((wait2-wait1)<4000);
  //delay(4000);

  
  mySerial.println("AT+SAPBR=3,1,Contype,GPRS");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<100);
  //delay(100);


  
  mySerial.println("AT+SAPBR=3,1,APN," + apn);
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<100);
  //delay(100);


  
  mySerial.println("AT+SAPBR=3,1,USER," + apn_u); //Comment out, if you need username
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<100);
  //delay(100);

  
  mySerial.println("AT+SAPBR=3,1,PWD," + apn_p); //Comment out, if you need password
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<100);
  //delay(100);

  
  mySerial.println("AT+SAPBR =1,1");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<100);
  //delay(100);

  
  mySerial.println("AT+SAPBR=2,1");
  runsl();
  time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<2000);
  //delay(2000);
  mySerial.println("AT+HTTPINIT");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<100);
  //delay(100);

  
  mySerial.println("AT+HTTPPARA=CID,1");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<100);
  //delay(100);

  
  mySerial.println("AT+HTTPPARA=URL," + url);
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<100);
  //delay(100);

  
  mySerial.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<100);
  //delay(100);

  
  mySerial.println("AT+HTTPDATA=192,10000");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<100);
  //delay(100);

  
  mySerial.println("params=" + data2);
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<10000);
  //delay(10000);

  
  mySerial.println("AT+HTTPACTION=1");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<5000);
  //delay(5000);

  i=0;
  j=0;
  count1=0;
  mySerial.println("AT+HTTPREAD");
  while (mySerial.available()) {
    bulb=mySerial.read();
    Serial.println(bulb);
    if(bulb==78){
      Serial.println("ssssssssssssssssss");
      i=j;
      count1++;
      Serial.println(i);
    }
   
   if((i==j+1)&&(bulb==79)){
    Serial.println("Bulb off"); 
    digitalWrite(7,LOW);
    EEPROM.write(0,0);     
   }
   
   
   i++; 
  }
  if(count1==2){
     Serial.println("Bulb on");
     digitalWrite(7,HIGH);
     EEPROM.write(0,1);      
  }
  time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     pulsecount();
  }
  }while((wait2-wait1)<100);
  //delay(100); 
  mySerial.println("AT+HTTPTERM");
  runsl(); 
}

//Print GSM Status
void runsl() {
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }

}



void pulsecount(){
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
    time1=time2;
    delay(200);
   }
   
      
