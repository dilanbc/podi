#include<EEPROM>

void setup() {
 Serial.begin(9600);

EEPROM.write(20,'a');
 

}

void loop() {
  // put your main code here, to run repeatedly:

}
