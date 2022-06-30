#include <WiFi.h>
#include "time.h"
#include <FirebaseESP32.h>

int orderno;
 
#define FIREBASE_HOST "https://esp-pager-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "ftr9PIfAclc6vp5XDwy9PtF0rhIZMitF7fRXSvLr"
#define WIFI_SSID "HUAWEI_HOME"
#define WIFI_PASSWORD "dilan12345"

const char* ntpServer = "pool.ntp.org";//time server
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 3600; 
 
//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

 
void setup()
{
 
  Serial.begin(115200);
 

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
 

  
 
  Serial.println("------------------------------------");
  Serial.println("Connected...");


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
    struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  
  
  
delay(100); 
  
  json.set("/Hour",timeinfo.tm_hour);
  json.set("/Min",timeinfo.tm_min);
  json.set("/Sec",timeinfo.tm_sec);
  json.set("/Table",0);
  json.set("/Food",0);
  json.set("/fback",0);

  char webdata[20];
  sprintf(webdata,"/Pager/%u/",1);
  Firebase.updateNode(firebaseData,webdata,json);
  json.clear();
  json.set("/ordern",orderno);
  Firebase.updateNode(firebaseData,"/Pager/",json);
}
