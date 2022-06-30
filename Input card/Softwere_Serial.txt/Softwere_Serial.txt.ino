#include <SoftwareSerial.h>
SoftwareSerial mySerial(8,9);  //rx,tx

void setup() 
{
  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial.println("hai");
}

void loop() {
  //mySerial.println("qwertyuiopasdfghjklzxcvbnm,.");
  delay(5000);
 while(mySerial.available()) {
    Serial.write(mySerial.read());
  }
  while(Serial.available()) {
    mySerial.write(Serial.read());
  }
}
