#include <LoRa.h>
#include <SPI.h>
#include <WiFi.h>
#include "time.h"
#include <FirebaseESP32.h>

#define ss 5
#define rst 16
#define dio0 17

#define yeskey 4
#define nokey 15

int orderno;

int value; 
#define FIREBASE_HOST "https://esp-pager-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "ftr9PIfAclc6vp5XDwy9PtF0rhIZMitF7fRXSvLr"
#define WIFI_SSID "M4D"
#define WIFI_PASSWORD "#methsara1#"

const char* ntpServer = "pool.ntp.org";//time server
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 3600; 
 
//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

 
void setup()
{
 pinMode(2,1);
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 // printLocalTime();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
 
  /*
  This option allows get and delete functions (PUT and DELETE HTTP requests) works for device connected behind the
  Firewall that allows only GET and POST requests.
  
  Firebase.enableClassicRequest(firebaseData, true);
  */
 
  //String path = "/data";
  
 
  Serial.println("------------------------------------");
  Serial.println("Connected...");

  LoRa.setPins(ss, rst, dio0);    //setup LoRa transceiver module
 
  while (!LoRa.begin(433E6))     //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  
  LoRa.setSyncWord(0xA5);
  Serial.println("LoRa Initializing OK!");
  if (Firebase.RTDB.getInt(&firebaseData,"/Pager/ordern")) {
  if (firebaseData.dataType() == "int") {
    int intValue = firebaseData.intData();
    orderno=intValue;
    Serial.println(intValue);
  }
}
  
}
 
void loop()
{
int packetSize = LoRa.parsePacket();    // try to parse packet
  if (packetSize) 
  {
   
    
    Serial.print("Received packet '");
 
    while (LoRa.available())              // read packet
    {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 

       value=LoRaData.toInt();

  

       if(value>0){
       

       orderno++;
           struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  
  
  
delay(100); 
  
  json.set("/Hour",timeinfo.tm_hour);
  json.set("/Min",timeinfo.tm_min);
  json.set("/Sec",timeinfo.tm_sec);
  json.set("/Table",value/100);
  json.set("/Food",value%100);
  json.set("/fback",0);

  char webdata[20];
  sprintf(webdata,"/Pager/%u/",orderno);
  Firebase.updateNode(firebaseData,webdata,json);
  json.clear();
  json.set("/ordern",orderno);
  Firebase.updateNode(firebaseData,"/Pager/",json);

       
       }
      
        

      
    }
   // Serial.print("' with RSSI ");         // print RSSI of packet
  
  Serial.println(LoRa.packetRssi());
  }

if(digitalRead(yeskey)){
  
  json.set("/fback",1);

  char webdata[20];
  sprintf(webdata,"/Pager/%u/",orderno);
  Firebase.updateNode(firebaseData,webdata,json);
  delay(500);
  LoRa.beginPacket();   
  LoRa.print("1");
  LoRa.endPacket();
   LoRa.beginPacket();   
  LoRa.print("1");
  LoRa.endPacket();
   LoRa.beginPacket();   
  LoRa.print("1");
  LoRa.endPacket();
   LoRa.beginPacket();   
 LoRa.print("1");
  LoRa.endPacket();
   LoRa.beginPacket();   
  LoRa.print("1");
  LoRa.endPacket();
   LoRa.beginPacket();   
  LoRa.print("1");
  LoRa.endPacket();
   LoRa.beginPacket();   
  LoRa.print("1");
  LoRa.endPacket();
   LoRa.beginPacket();   
  LoRa.print("1");
  LoRa.endPacket();
   LoRa.beginPacket();   
  LoRa.print("1");
  LoRa.endPacket();
  LoRa.beginPacket();   
  LoRa.print("1");
  LoRa.endPacket();
  delay(500);
  }
  
  if(digitalRead(nokey)){
  LoRa.beginPacket();   
  LoRa.print("0");
  LoRa.endPacket();
  LoRa.beginPacket();   
   LoRa.print("0");
  LoRa.endPacket();LoRa.beginPacket();   
   LoRa.print("0");
  LoRa.endPacket();LoRa.beginPacket();   
   LoRa.print("0");
  LoRa.endPacket();LoRa.beginPacket();   
   LoRa.print("0");
  LoRa.endPacket();LoRa.beginPacket();   
   LoRa.print("0");
  LoRa.endPacket();LoRa.beginPacket();   
   LoRa.print("0");
  LoRa.endPacket();LoRa.beginPacket();   
   LoRa.print("0");
  LoRa.endPacket();LoRa.beginPacket();   
   LoRa.print("0");
  LoRa.endPacket();LoRa.beginPacket();   
   LoRa.print("0");
  LoRa.endPacket();LoRa.beginPacket();   
   LoRa.print("0");
  LoRa.endPacket();LoRa.beginPacket();   
   LoRa.print("0");
  LoRa.endPacket();
 delay(500);
  }
 

 
}
