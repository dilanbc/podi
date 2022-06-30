unsigned long number;
void setup() {
Serial.begin(9600);
}

void loop() {
 while(Serial.available()>0){
  number=Serial.parseInt();
  Serial.print("entered number  - ");
  Serial.println(number);                 //print number
  Serial.print(number/16777216);             //devide by 256^3
  Serial.print("   ");
  Serial.print((number%16777216)/65536);      //devide balance by 256^2
  Serial.print("   ");
  Serial.print(((number%16777216)%65536)/256);      //balance
  Serial.print("  ");
  Serial.print(((number%16777216)%65536)%256);      //balance
  Serial.println("  ");
  Serial.println();
  
 }
Serial.println("last");
}
