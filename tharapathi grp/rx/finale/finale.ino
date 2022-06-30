#include <LoRa.h>
#include <SPI.h>
#include <WiFi.h>
#include "time.h"
#include <FirebaseESP32.h>
#include "LCDIC2.h"

LCDIC2 lcd(0x27, 16, 2);

#define ss 5
#define rst 16
#define dio0 17

#define upkey 2
#define downkey 15
#define selectkey 4

#define foodlim 10
String foodst [foodlim]={"Special Rice","Chicken Mix.","Seafood Mix. ","Nasi Goreng ","Chicken Bir.","Chicken Kot.","Seafood Kot.","Cheese Kott.","Chicken Nood ","Seafood Nood"};

int orderno;
int tempOr=1;
int value; 
#define FIREBASE_HOST "https://hogwarts-16731-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "lE9ae0fzcQnHDYuYvUhVZMIklZtGzxvZU7QP4klx"
#define WIFI_SSID "HUAWEI_HOME"
#define WIFI_PASSWORD "dilan12345"

const char* ntpServer = "pool.ntp.org";//time server
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 3600; 
 
//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

char lcddata[30];
 
void setup()
{
 pinMode(2,1);
  Serial.begin(115200);
  lcd.begin();
  lcd.setCursor(0);//off cursor
  while (!Serial);
  Serial.println("LoRa Receiver");
  lcd.setCursor(4,0);//off cursor
  lcd.print("WELCOME");

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
  lcd.clear();

sprintf(lcddata,"Select order>%02d",tempOr);
  lcd.setCursor(0,0);
  lcd.print(lcddata);
  
}
 
void loop()
{
  

  if(digitalRead(upkey)){
    
   if(orderno>tempOr){
    tempOr++;
     sprintf(lcddata,"Select order>%02d",tempOr);
   lcd.setCursor(0,0);
  lcd.print(lcddata);
  delay(500);
   }
  
   }

   if(digitalRead(downkey)){
    
   if(tempOr>1){
    tempOr--;
     sprintf(lcddata,"Select order>%02d",tempOr);
   lcd.setCursor(0,0);
  lcd.print(lcddata);
  delay(500);
   }
  
   }

if(digitalRead(selectkey)){
char text[4][10]={"Do nothi.","Order R ","Please W","Sorry   "};
int msg=0; 
sprintf(lcddata,"Messege>%s",text[msg]);
lcd.setCursor(0,1);
lcd.print(lcddata);
delay(1000);

while(1){
if(digitalRead(upkey)){
  if(msg<3){
    msg++;
   sprintf(lcddata,"Messege>%s",text[msg]);
lcd.setCursor(0,1);
lcd.print(lcddata);
delay(500); 
  }
  
}

if(digitalRead(downkey)){
  if(msg>0){
    msg--;
   sprintf(lcddata,"Messege>%s",text[msg]);
lcd.setCursor(0,1);
lcd.print(lcddata);
delay(500); 
  }
  
}
///////////////////////////////////////////////////////////////////
if(digitalRead(selectkey)){
lcd.clear();  
lcd.setCursor(0,0);
lcd.print("Please wait. .");

   json.set("/fback",msg);

  char webdata[20];
  sprintf(webdata,"/Pager/%u/",tempOr);
  Firebase.updateNode(firebaseData,webdata,json);
  json.clear();
  Firebase.updateNode(firebaseData,"/Pager/",json);

lcd.clear();  
lcd.setCursor(0,0);
lcd.print("updated");


sprintf(lcddata,"%d",(tempOr*10)+msg);
for(int i=0;i<20;i++){
 LoRa.beginPacket();   
LoRa.print(lcddata);
LoRa.endPacket(); 
}



delay(500); 
sprintf(lcddata,"Select order>%02d",tempOr);
  lcd.setCursor(0,0);
  lcd.print(lcddata);
 break;
 
  
}//select key
  
}
    
 
  
}
  
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
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("New Order");
  
  sprintf(lcddata,"%s",foodst[value-1]);
  lcd.setCursor(0,1);
  lcd.print(lcddata);
  delay(500); 
  
  

  
  json.set("/Hour",timeinfo.tm_hour);
  json.set("/Min",timeinfo.tm_min);
  json.set("/Sec",timeinfo.tm_sec);
  json.set("/Food",value);
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
delay(4000);
lcd.clear();
  sprintf(lcddata,"Select order>%02d",tempOr);
  lcd.setCursor(0,0);
  lcd.print(lcddata);

  
  }


 
}
