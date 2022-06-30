int v2;
char letter;
void setup() {

  Serial.begin(115200);
  englishletter1();
 
}


void loop() {
 
   

  
  }
  



 


int englishletter1(){
  do{
    v2 =map(analogRead(A0),0,1023,1,9);
  switch (v2){
    case 1:  Serial.println("a b c"); break;
    case 2:  Serial.println("d e f"); break;
    case 3: Serial.println("g h i");break;
    case 4:  Serial.println("j k l");break;
    case 5:  Serial.println("m n o");break;
    case 6:  Serial.println("p q r");break;
    case 7:  Serial.println("s t u");break;
    case 8:  Serial.println("v w x");break;
    case 9:  Serial.println("y z # ");break;}
    delay(100);

    }while((analogRead(A3)<900)&&(analogRead(A2)<900)&&(analogRead(A1)<900));
if(analogRead(A3)>900){
   switch(v2){
  case 1: letter='a'; break;
  case 2: letter='d'; break;
  case 3: letter='g'; break;
  case 4: letter='j'; break;
  case 5: letter='m'; break;
  case 6: letter='p'; break;
  case 7: letter='s'; break;
  case 8: letter='v'; break;
  case 9: letter='y'; break; }
  }
if(analogRead(A2)>900){
   switch(v2){
  case 1: letter='b'; break;
  case 2: letter='e'; break;
  case 3: letter='h'; break;
  case 4: letter='k'; break;
  case 5: letter='n'; break;
  case 6: letter='q'; break;
  case 7: letter='t'; break;
  case 8: letter='w'; break;
  case 9: letter='z'; break; }
  }
if(analogRead(A1)>900){
   switch(v2){
  case 1: letter='c'; break;
  case 2: letter='f'; break;
  case 3: letter='i'; break;
  case 4: letter='l'; break;
  case 5: letter='o'; break;
  case 6: letter='r'; break;
  case 7: letter='u'; break;
  case 8: letter='x'; break;
  case 9: letter=' '; break; }
  } 
 Serial.println(letter);
 delay(2000);  
}















  



