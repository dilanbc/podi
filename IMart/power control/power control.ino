//For more Information visit www.aeq-web.com?ref=arduinoide

#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(9, 8);            // RX, TX Pins
String apn = "dialogbb";                       //APN
String apn_u = "";                     //APN-Username
String apn_p = "";                     //APN-Password
String url = "http://imart.a4tours.net/updtMtr.php";  //URL for HTTP-POST-REQUEST
String data1;   //String for the first Paramter (e.g. Sensor1)
String data2;   //String for the second Paramter (e.g. Sensor2)

int bulb;
int i=0;
int j;
int count =0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  Serial.println("IK");
  delay(10000);
  pinMode(13,OUTPUT);
}

void loop() { // run over and over

  
    data1 = "25";
    data2 = "25000";
    gsm_sendhttp(); //Start the GSM-Modul and start the transmisson
    delay(20000);

}


void gsm_sendhttp() {
  
  
 
  mySerial.println("AT");
  runsl();//Print GSM Status an the Serial Output;
  delay(4000);
  mySerial.println("AT+SAPBR=3,1,Contype,GPRS");
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR=3,1,APN," + apn);
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR=3,1,USER," + apn_u); //Comment out, if you need username
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR=3,1,PWD," + apn_p); //Comment out, if you need password
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR =1,1");
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR=2,1");
  runsl();
  delay(2000);
  mySerial.println("AT+HTTPINIT");
  runsl();
  delay(100);
  mySerial.println("AT+HTTPPARA=CID,1");
  runsl();
  delay(100);
  mySerial.println("AT+HTTPPARA=URL," + url);
  runsl();
  delay(100);
  mySerial.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  runsl();
  delay(100);
  mySerial.println("AT+HTTPDATA=192,10000");
  runsl();
  delay(100);
  mySerial.println("params=" + data2);
  runsl();
  delay(10000);
  mySerial.println("AT+HTTPACTION=1");
  runsl();
  delay(5000);
  i=0;
  j=0;
  count=0;
  mySerial.println("AT+HTTPREAD");
  while (mySerial.available()) {
    bulb=mySerial.read();
    Serial.println(bulb);
    if(bulb==78){
      Serial.println("ssssssssssssssssss");
      i=j;
      count++;
      Serial.println(i);
    }
   
   if((i==j+1)&&(bulb==79)){
    Serial.println("Bulb off");     
   }
   
   
   i++; 
  }
  if(count==2){
     Serial.println("Bulb on");     
  }
  delay(100);
  mySerial.println("AT+HTTPTERM");
  runsl(); 

}


//Print GSM Status
void runsl() {
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }

}
