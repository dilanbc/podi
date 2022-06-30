

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#include <SPI.h>
#include <MFRC522.h>
#include <RFID.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
int buzzer = D0;


constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

String tag;

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

/* 1. Define the WiFi credentials */
#define WIFI_SSID "SLT-HOME-WIFI"
#define WIFI_PASSWORD "dilan12345"

/* 2. Define the API Key */
#define API_KEY "AIzaSyBOx85aZrxzHo2v3m7RVKyqwgLoUKgSRq4"

/* 3. Define the RTDB URL */
#define DATABASE_URL "ex-pay-5dae4-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL ""
#define USER_PASSWORD ""

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

String rfcard[3]={"1812811552","3610623543","511061213"};

int count = 0;
int chargeV=100;///////////////////////////////////////////////////////////////
void setup()
{

  Serial.begin(9600);
  lcd.begin(16,2);
   lcd.setCursor(0,0);
   lcd.print("Connecting to Wi-Fi");
   lcd.setCursor(5,1);
   lcd.print("Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  lcd.clear();
  Serial.print("Connected with IP: ");
  lcd.setCursor(0,0);
  lcd.print("Connected with IP:");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

 
  /* Assign the api key (required) */
  config.api_key = API_KEY;

 

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  //config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  //Or use legacy authenticate method
  //config.database_url = DATABASE_URL;
 config.signer.tokens.legacy_token = "fXb2gG2dYpDOi9s3QX8uOCbjRSv6JFXy1aNdVlWh";

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);
   lcd.clear();
   lcd.print("TAP THE");
   lcd.setCursor(0,1);
   lcd.print("RFID CARD");
}

void loop()
{
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (Firebase.ready()&&rfid.PICC_ReadCardSerial())
  {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    byte j=3;
    for(byte i=0;i<4;i++){
      if(tag==rfcard[i]){j=i;}
    }
    if(j<3){
        
  if(j==0){


    Serial.printf("Set state... %s\n", Firebase.RTDB.setInt(&fbdo, "/A/state", false) ? "ok" : fbdo.errorReason().c_str());

   //Serial.printf("Get credit... %s\n", Firebase.RTDB.getInt(&fbdo, "/A/credit") ? String(fbdo.intData()).c_str() : fbdo.errorReason().c_str());
  if(Firebase.RTDB.getInt(&fbdo, "/A/credit")){
    int creditb= fbdo.intData();
    if(creditb<chargeV){
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("You have ride "); 
     lcd.setCursor(0,1);
     lcd.print("by credit"); 
     tone(buzzer,2000,100);
        delay(1);
    }
    else{
      Serial.println("Access Granted!");
      lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Access Granted");
        lcd.setCursor(0,1);
        lcd.print("GATE OPEN");
        tone(buzzer, 4000,100);
        delay(1);
      Serial.printf("Set credit... %s\n", Firebase.RTDB.setInt(&fbdo, "/A/credit", creditb-chargeV) ? "ok" : fbdo.errorReason().c_str());
    }
   
  }
  }

  

if(j==1){
    Serial.printf("Set state... %s\n", Firebase.RTDB.setInt(&fbdo, "/B/state", false) ? "ok" : fbdo.errorReason().c_str());

  
    if(Firebase.RTDB.getInt(&fbdo, "/B/credit")){
    int creditb= fbdo.intData();
  if(creditb<chargeV){
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("You have ride "); 
     lcd.setCursor(0,1);
     lcd.print("by credit"); 
     tone(buzzer,2000,100);
        delay(1);
    }
    else{
      Serial.println("Access Granted!");
      lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Access Granted");
        lcd.setCursor(0,1);
        lcd.print("GATE OPEN");
        tone(buzzer, 4000,100);
        delay(1);
       Serial.printf("Set credit... %s\n", Firebase.RTDB.setInt(&fbdo, "/B/credit", creditb-chargeV) ? "ok" : fbdo.errorReason().c_str());
    }
    
  
  }
    }

if(j==2){
    Serial.printf("Set state... %s\n", Firebase.RTDB.setInt(&fbdo, "/C/state", false) ? "ok" : fbdo.errorReason().c_str());

  
    if(Firebase.RTDB.getInt(&fbdo, "/C/credit")){
    int creditb= fbdo.intData();
    if(creditb<chargeV){
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("You have ride "); 
     lcd.setCursor(0,1);
     lcd.print("by credit");
     tone(buzzer,2000,100);
        delay(1);
    }
    else{
      Serial.println("Access Granted!");
      lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Access Granted");
        lcd.setCursor(0,1);
        lcd.print("GATE OPEN");
        tone(buzzer, 4000,100);
        delay(1);
      Serial.printf("Set credit... %s\n", Firebase.RTDB.setInt(&fbdo, "/C/credit", creditb-chargeV) ? "ok" : fbdo.errorReason().c_str()); 
    }
  
  }
    }

   

    FirebaseJson json;

    delay(10000);
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("TAP THE");
        lcd.setCursor(0,1);
        lcd.print("RFID CARD");
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    else{
      Serial.println("Access Denied!");
       lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Access Denied");
          
        tone(buzzer,2000,100);
        delay(1);
        delay(10000);
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("TAP THE");
        lcd.setCursor(0,1);
        lcd.print("RFID CARD");
    }






    
    
  //  json.add("value", count);

   /* Serial.printf("Push json... %s\n", Firebase.RTDB.pushJSON(&fbdo, "/Users/push", &json) ? "ok" : fbdo.errorReason().c_str());

    json.set("value", count + 100);
    Serial.printf("Update json... %s\n\n", Firebase.RTDB.updateNode(&fbdo, String("/Users/push/" + fbdo.pushName()).c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
    */
   // count++;
     tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
