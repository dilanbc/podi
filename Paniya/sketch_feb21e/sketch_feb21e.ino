unsigned long time;
unsigned long count;
unsigned long timerValue=10;  //enter in second
int timerValue1=10;
bool pauseState=0;

byte rst=8;
byte pause=9;
byte on=10;

void setup() {
  Serial.begin(9600);
  while(digitalRead(on)==0){
    if(digitalRead(on)==1){
      break;
    }
  }
  time = millis();
 timerValue=(time/1000)+timerValue;
}

void loop() {
 
 
 
 if(pauseState==0){
  time = millis();
 count=timerValue-(time/1000);
 }
 else{
  timerValue=timerValue+
 }
 
 
 
 showValue(count);
 
if(count==0){                    //auto reset count
  timerValue+=timerValue1;
 }
 if(digitalRead(rst)==1){       //rst button
  time = millis();
 timerValue=(time/1000)+timerValue1;
 }
 if(digitalRead(pause)==1){       //pause button
  delay(500);
  pauseState=1;
 }
}

void showValue(byte value){
  byte digit1=value/60;
  byte digit2=(value%60)/10;
  byte digit3=(value%60)%10;

  Serial.print(digit1);
  Serial.print(":");
  Serial.print(digit2);
  Serial.print(":");
  Serial.println(digit3);
    
}

