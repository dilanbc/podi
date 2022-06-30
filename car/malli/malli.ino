#include <Servo.h>

byte trigPin=A0;
byte echoPin=A1;
Servo myservo;  // create servo object to control a servo
void setup() {
  Serial.begin(9600);
  myservo.attach(9);  // (pin, min, max);
  pinMode(trigPin,1);
  pinMode(echoPin,0);
  pinMode(7,1);
  pinMode(6,1);
  pinMode(5,1);
  pinMode(4,1);
  servolk(90);
  //forward();
  //right();
 // left();
 off();
}
void loop() {
 forward();
 float distance=servolk(90);
 Serial.println(distance);
 if( distance<30){
  off();
  float rightm=servolk(0);//right
  delay(500);
  float leftm=servolk(180);
  delay(500);
  if(rightm>leftm){
    right();
  }
  else{
    left();
  }
  delay(100);
  
 }
  delay(100);                     
}

float servolk(byte angle){
 myservo.write(angle);
 digitalWrite(trigPin, LOW);
 delayMicroseconds(5);
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 pinMode(echoPin, INPUT);
int duration = pulseIn(echoPin, HIGH);
float cm = (duration/2) / 29.1;  
 return cm;     
 }

void forward(){
  digitalWrite(4,1);
  digitalWrite(6,1);
  digitalWrite(5,0);
  digitalWrite(7,0);
  
}

void right(){
  digitalWrite(4,1);
  digitalWrite(6,0);
  digitalWrite(5,0);
  digitalWrite(7,1);
}

void left(){
digitalWrite(4,0);
  digitalWrite(6,1);
  digitalWrite(5,1);
  digitalWrite(7,0);
  
}
void off(){
  digitalWrite(4,0);
  digitalWrite(6,0);
  digitalWrite(5,0);
  digitalWrite(7,0);
}


 
