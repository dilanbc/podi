#include <EEPROM.h>


void setup() {
  Serial.begin(9600);
  for(int i = 0;i<50;){
    Serial.print(i);
    Serial.print("  - ");
    Serial.print(EEPROM.read(i));
    Serial.println();
    delay(20);
    i++;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
