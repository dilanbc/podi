

#include <Servo.h>

#include <AFMotor.h>

AF_DCMotor motorr(4);
AF_DCMotor motorl(3);
byte motorS=150;
int trigPin = A0;
int echoPin = A1;
Servo myservo1;  
Servo myservo2; 
int frontside;
int rightside;
int leftside;

byte left2=180;
byte left1=90;

byte right2=0;
byte right1=70;

byte front2=75;
byte front1=90;
int delay1=1;
long duration, cm;
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo1.attach(9);  
  myservo2.attach(10); 
  myservo2.write(front2);  
  myservo1.write(front1);
  delay(500);
  lookfast();
}

void loop() {
   look();
   while(cm>50){
    motorr.run(FORWARD);
    motorl.run(FORWARD);
    motorr.setSpeed(motorS);
    motorl.setSpeed(motorS);
    look();    
    delay(10); 
  }
  
  if(cm<=50){
    motorr.run(RELEASE);
    motorl.run(RELEASE);
   lookfast();
   turn(); 
  }
                        
}


void lookfast(){
 myservo1.write(right1);
 myservo2.write(right2);
 delay(500);
 digitalWrite(trigPin, LOW);
 delayMicroseconds(5);
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 pinMode(echoPin, INPUT);
 duration = pulseIn(echoPin, HIGH);
 cm = (duration/2) / 29.1;     
 rightside=cm;
 myservo1.write(left1);
 myservo2.write(left2);
 delay(500);
 digitalWrite(trigPin, LOW);
 delayMicroseconds(5);
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 pinMode(echoPin, INPUT);
 duration = pulseIn(echoPin, HIGH);
 cm = (duration/2) / 29.1;     
 leftside=cm;
 myservo1.write(front1);
 myservo2.write(front2);
 delay(500);
 }

 void turn(){
  int sight1;
  int sight2;
  if((leftside<20)&&(rightside<20)){
   while(1); 
  }
  if(leftside<rightside){//turnright;
    motorr.run(BACKWARD);
    motorl.run(BACKWARD);
    motorr.setSpeed(100);
    motorl.setSpeed(100);
    delay(2000);
    motorr.setSpeed(0);
    motorl.setSpeed(0);
    motorr.run(FORWARD);
    motorl.run(BACKWARD);
    motorr.setSpeed(100);
    motorl.setSpeed(100);
    delay(1000);
  }
  if(leftside>rightside){ //turnleft
    motorr.run(BACKWARD);
    motorl.run(BACKWARD);
    motorr.setSpeed(100);
    motorl.setSpeed(100);
    delay(2000);
    motorr.setSpeed(0);
    motorl.setSpeed(0);
    motorl.run(FORWARD);
    motorr.run(BACKWARD);
    motorr.setSpeed(100);
    motorl.setSpeed(100);
    delay(1000);
  if(leftside==rightside){//turnleft
    
  }
 }
 }
 void look(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;     
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
 }

