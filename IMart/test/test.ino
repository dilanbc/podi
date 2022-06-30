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

unsigned long time;  //time
unsigned long time1; 
unsigned long time2;
unsigned long time3;
unsigned long server1=0;  //server update delay
unsigned long server2;    //server update delay
unsigned long wait1=0;    //
unsigned long wait2;
bool power=false;
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


unsigned int writevalue;
float readvalue;
unsigned long writevalue1;
float readvalue1;


byte BL[8] = 
{
  B00000,
  B00000,
  B11111,
  B10000,
  B10101,
  B10000,
  B11111,
  B00000
};
byte BR[8] =
{
  B00000,
  B00000,
  B11110,
  B00011,
  B01011,
  B00011,
  B11110,
  B00000
};
byte PL[8] =
{
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000
};
byte PR[8] =
{
  B00000,
  B01100,
  B01111,
  B11100,
  B11100,
  B01111,
  B01100,
  B00000
};



void setup() {
  
  lcd.createChar(1,BL);
  lcd.createChar(2,BR);
  lcd.createChar(3,PL);
  lcd.createChar(4,PR);
  lcd.begin(20,4);
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(7,OUTPUT);     // power control
  digitalWrite(7,EEPROM.read(0));
  
  pinMode(10,OUTPUT);  //power on gsm module
  digitalWrite(10,HIGH);
  delay(1000);
  digitalWrite(10,LOW);  
 
  lcd.setCursor(4,0);
 
  lcd.print("IMart Meter");
  
  readdata();
  total=readvalue1;
  data2 = String(total,5);
  displayvalue();
////////////////////////////////////////

  do{
 powercheck();   
 time=millis();
 server2=time;     
  
 if((server2-server1)>20000){
 
     
 server1=server2;
  mySerial.println("AT");
  runsl();
  
  time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  Serial.println(analogRead(A0));
  if(analogRead(A0)>300){
     break;
  }
  
  }while((wait2-wait1)<4000);
 if(analogRead(A0)>300){
     break;
  }

  
  mySerial.println("AT+SAPBR=3,1,Contype,GPRS");
  }
  runsl();
  time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
    break;
  }
  }while((wait2-wait1)<100);
 if(analogRead(A0)>300){
     break;
  } 
 

  mySerial.println("AT+SAPBR=3,1,APN," + apn);
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
    break;
  }
  }while((wait2-wait1)<100);
  //delay(100);
  if(analogRead(A0)>300){
     break;
  }


  mySerial.println("AT+SAPBR=3,1,USER," + apn_u); //Comment out, if you need username
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
    break;
  }
  }while((wait2-wait1)<100);
  //delay(100);
  if(analogRead(A0)>300){
     break;
  }

  mySerial.println("AT+SAPBR=3,1,PWD," + apn_p); //Comment out, if you need password
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
    break;
  }
  }while((wait2-wait1)<100);
  //delay(100);
if(analogRead(A0)>300){
     break;
  }

  

  mySerial.println("AT+SAPBR =1,1");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
     break;
  }
  }while((wait2-wait1)<100);
  //delay(100);
if(analogRead(A0)>300){
     break;
  }



  mySerial.println("AT+SAPBR=2,1");
  runsl();
  time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
   break;  }
  }while((wait2-wait1)<2000);
  //delay(2000);
if(analogRead(A0)>300){
     break;
  }


  

  mySerial.println("AT+HTTPINIT");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
    break;
  }
  }while((wait2-wait1)<100);
  //delay(100);
if(analogRead(A0)>300){
     break;
  }

  
  

  mySerial.println("AT+HTTPPARA=CID,1");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
    break;
  }
  }while((wait2-wait1)<100);
  //delay(100);
if(analogRead(A0)>300){
     break;
  }

  

  mySerial.println("AT+HTTPPARA=URL," + url);
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
   break;
  }
  }while((wait2-wait1)<100);
  //delay(100);
if(analogRead(A0)>300){
     break;
  }

  

  mySerial.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
  break;
  }
  }while((wait2-wait1)<100);
  //delay(100);
if(analogRead(A0)>300){
     break;
  }

  

{
  mySerial.println("AT+HTTPDATA=192,10000");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
   break;
  }
  }while((wait2-wait1)<100);
  //delay(100);
if(analogRead(A0)>300){
     break;
  }


  
 
  mySerial.println("params=" + data2);
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
   break;
  }
  }while((wait2-wait1)<10000);
  //delay(10000);
if(analogRead(A0)>300){
     break;
  }


  
  
  mySerial.println("AT+HTTPACTION=1");
  runsl();
    time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
  break;
  }
  }while((wait2-wait1)<5000);
  //delay(5000);
if(analogRead(A0)>300){
     break;
  }



  
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
    lcd.setCursor(0,3);
    lcd.print("Disconnected");    
   }
   
   
   i++; 
  }
  if(count1==2){
     Serial.println("Bulb on");
     digitalWrite(7,HIGH);
     EEPROM.write(0,1); 
     lcd.setCursor(0,3);
     lcd.print("            ");
     lcd.setCursor(0,3);
     lcd.print("Connected");
               
  }
  time = millis();
  wait1=time;
  do{
  time = millis();
  wait2=time;
  if(analogRead(A0)>300){
   break;
  }
  }while((wait2-wait1)<100);
  //delay(100);
if(analogRead(A0)>300){
     break;
  }


  
   
  mySerial.println("AT+HTTPTERM");
  runsl(); 
 
 } // time delay open
 }while(analogRead(A0)<300);

/////////////////////////////////////////////////////////////////////////////  
   Serial.println(analogRead(A0));
   time = millis();
   time1=time;
   Serial.print("time1-");
   Serial.println(time1);
   delay(200);
   server1=0;
} //setup close

////////////////////////////////////////////////////////////////////////////////
void loop() {
  time = millis();
  server2=time;
  powercheck();

 
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



void gsm_sendhttp() {
  
  if(analogRead(A1)>700){
 
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
    lcd.setCursor(0,3);
    lcd.print("Disconnected");      
   }
   
   
   i++; 
  }
  if(count1==2){
     Serial.println("Bulb on");
     digitalWrite(7,HIGH);
     EEPROM.write(0,1); 
     lcd.setCursor(0,3);
     lcd.print("            ");
     lcd.setCursor(0,3);
     lcd.print("Connected");     
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
void writedata(){
 Serial.print("total - ");
 writevalue=total;
 EEPROM.write(7,(writevalue/65536));
 EEPROM.write(6,(writevalue%65536)/256);
 EEPROM.write(5,((writevalue/65536)%256)); 
 
 writevalue1=(total-writevalue)*10000000;  
 Serial.println(writevalue1);
 EEPROM.write(4,writevalue1/16777216);
 EEPROM.write(3,(writevalue1%16777216)/65536);
 EEPROM.write(2,((writevalue1%16777216)%65536)/256);  
 EEPROM.write(1,((writevalue1%16777216)%65536)%256);
}

void readdata(){
 readvalue= (((EEPROM.read(7)*65536)+(EEPROM.read(6)*256)+(EEPROM.read(5))));
 readvalue1=((EEPROM.read(4)*16777216)+(EEPROM.read(3)*65536)+(EEPROM.read(2)*256)+(EEPROM.read(1)));
 readvalue1= readvalue1/10000000;
 Serial.print("read value 1 - ");

 Serial.println(readvalue1,7);
 readvalue1=readvalue+readvalue1;
  Serial.print("read value - ");

 Serial.println(readvalue1,7);
}
 void displayvalue(){
    lcd.setCursor(0,2);
    lcd.print("                    ");
    
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
           
 }


void powercheck(){
  if(analogRead(A1)>700){
   if(power==true){
   lcd.setCursor(18,3);
   lcd.write(3);
   lcd.setCursor(19,3);
   lcd.write(4);
   pinMode(10,OUTPUT);  //power on gsm module
  digitalWrite(10,HIGH);
  delay(1000);
  digitalWrite(10,LOW);  
   power=false;  
   }
   
  }
  else{
  if(power==false){
  lcd.setCursor(18,3);
  lcd.write(1);
  lcd.setCursor(19,3);
  lcd.write(2);
  writedata();
  power=true;
  Serial.println("writed"); 
  }
     
  }
  
}

