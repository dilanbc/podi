
#include <AFMotor.h>

AF_DCMotor motorr(4);
AF_DCMotor motorl(3);
void setup() {
  Serial.begin(9600);          
  
}

void loop() {
    
  motorr.run(FORWARD);
  for (int i=0; i<255; i++) {
    motorr.setSpeed(i);  
    delay(10);
 }
 
  for (int i=255; i!=0; i--) {
    motorr.setSpeed(i);  
    delay(10);
 }
 motorr.run(RELEASE);
 
  motorl.run(FORWARD);
  for (int i=0; i<255; i++) {
    motorl.setSpeed(i);  
    delay(10);
 }
 
  for (int i=255; i!=0; i--) {
    motorl.setSpeed(i);  
    delay(10);
 } 
 motorl.run(RELEASE);

 
}
