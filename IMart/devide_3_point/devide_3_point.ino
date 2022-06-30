long number;
void setup() {
Serial.begin(9600);
}

void loop() {
 while(Serial.available()>0){
  number=Serial.parseInt();
  Serial.print("entered number  - ");
  Serial.println(number);                 //print number
  Serial.print(number/65536);             //devide by 255^3
  Serial.print("   ");
  Serial.print((number%65536)/256);      //devide balance by 255^2
  Serial.print("   ");
  Serial.println(((number/65536)%256));      //balance
  Serial.print("  ");
  Serial.println();
  
 }

}
