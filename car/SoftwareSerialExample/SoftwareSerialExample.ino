
#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 8); // RX, TX
char letter;
void setup() {
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(6,OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  Serial.println("Starting");
  mySerial.begin(9600);
}

void loop() { // run over and over
  if (mySerial.available()) {

    letter=mySerial.read();
    Serial.println(letter);

     if(letter=='a'){
      Serial.println("left");
       digitalWrite(9,LOW);
       digitalWrite(10,HIGH);
       digitalWrite(11,HIGH);
       digitalWrite(6,LOW);
     }
if(letter=='d'){
      Serial.println("right");
       digitalWrite(9,HIGH);
       digitalWrite(10,LOW);
       digitalWrite(11,LOW);
       digitalWrite(6,HIGH);
     }

if(letter=='w'){
      Serial.println("forward");
       digitalWrite(9,HIGH);
       digitalWrite(10,LOW);
       digitalWrite(11,HIGH);
       digitalWrite(6,LOW);
     }
if(letter=='s'){
      Serial.println("back");
       digitalWrite(9,LOW);
       digitalWrite(10,HIGH);
       digitalWrite(11,LOW);
       digitalWrite(6,HIGH);
     }

if(letter=='p'){
      Serial.println("stop");
       digitalWrite(9,LOW);
       digitalWrite(10,LOW);
       digitalWrite(11,LOW);
       digitalWrite(6,LOW);
     }
if(letter=='g'){
      Serial.println("right");
       digitalWrite(9,HIGH);
       digitalWrite(10,LOW);
       digitalWrite(11,LOW);
       digitalWrite(6,HIGH);
     }

    
  }
  
}
