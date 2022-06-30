#include <BigNumber.h>

unsigned long time;
unsigned long time1;
unsigned long time2;
float mpr=3200.0000000;
float diffrent;
float watt;
float pf;
float unit;
float total=0;

void setup() {
  
  Serial.begin(9600);
}


void loop() {
 
   Serial.println( analogRead(A0));
   if(analogRead(A0)>300){
    time = millis();
    time1=time;
    Serial.println(time1);
    delay(200);
    do{
      
    }while(analogRead(A0)<300);
    time = millis();
    time2=time;
    Serial.println(time2);
    Serial.print("diffrent-");
    diffrent=time2-time1;
    diffrent=diffrent/1000;
    Serial.println(diffrent);
    watt=(mpr*diffrent);  
    watt=(3600000/watt);
    Serial.print("watt-");
    Serial.println(watt);

    Serial.print("Power Factor");
    pf=(watt*diffrent/3600);
    Serial.println(pf);

    unit=((pf)/(1000.0000*diffrent));
    Serial.print("unit-");
    Serial.println(unit);
    
    Serial.print("total-");
    total=total+(unit*diffrent);
    Serial.println(total,10);
    
    delay(5000);
   }   
  delay(1);        // delay in between reads for stability
}
