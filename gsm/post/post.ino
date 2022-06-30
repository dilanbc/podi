#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(8, 9);            // RX, TX Pins
String apn = "hutch3g";                       //APN
String apn_u = "";                     //APN-Username
String apn_p = "";                     //APN-Password
String url = "http://powerpanel123.000webhostapp.com/writepost.php";  //URL for HTTP-POST-REQUEST
String data; //String for the first Paramter (e.g. Sensor1)
byte numdata=0;
float vol;
float amp;


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Started");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
 delay(10000);
    mySerial.println("AT");
  runsl();//Print GSM Status an the Serial Output;
  delay(4000);
  mySerial.println("AT+SAPBR=3,1,Contype,GPRS");
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR=3,1,APN," + apn);
  runsl();
  delay(100);

}

void loop() { // run over and over
    numdata++;
    vol+=0.3;
    amp+=0.2;
    if(numdata==8){
      numdata=0;
    }
    if(vol>300){
    vol=0;  
    }
    if(amp>50){
      amp=0;
    }
    
    data=String(numdata,BIN);
    data="L"+data+"V"+String(vol)+"A"+String(amp)+"E";
    Serial.println(data);
    
        
    //L000V34.2A585E
    gsm_sendhttp(); //Start the GSM-Modul and start the transmisson
    delay(10000); //Wait one minute

}


void gsm_sendhttp() {
  
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
  mySerial.println("AT+HTTPDATA=20,10000");
  runsl();
  delay(100);
  mySerial.println("up="+data);
  runsl();
  delay(10000);
  
  mySerial.println("AT+HTTPACTION=1");
  runsl();
  delay(5000);
  mySerial.println("AT+HTTPREAD");
  runsl();
  delay(100);
  mySerial.println("AT+HTTPTERM");
  runsl(); 
  delay(100);
 // mySerial.println("AT+SAPBR =0,1");
 // runsl(); 
 // delay(100);
}

//Print GSM Status
void runsl() {
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }

}
