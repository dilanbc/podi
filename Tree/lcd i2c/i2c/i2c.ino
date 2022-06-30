#include <Wire.h>
//include i2c library


void setup() {
Wire.begin();
//enable i2c registers
//^^step 1

Wire.beginTransmission(0x27);
//Call to i2c module address
//^^Step 2

Wire.write(85);
//Write value to device;
//^^Step 3


Wire.endTransmission();
//End the Transmission
//^^ Step 4
}

void loop() {
 }


