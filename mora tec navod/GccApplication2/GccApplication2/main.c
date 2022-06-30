#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD.h"
#include "I2C.h"

void Readtime();
void servoangle(uint8_t pin);
void setTime(uint8_t hSet,uint8_t mSet,uint8_t sSet );
uint8_t dayC;
uint8_t hourC;
uint8_t minC;
uint8_t secC;

uint8_t tabtimes[3][2]={{8,30},{12,15},{5,30}};
char lcddata[20];


int main(void)
{	TWIInit();
    LcdInit();
	_delay_ms(1000);
	
	DDRD=0xFF;
	
	setTime(5,29,55);
    while (1) 
    {	Readtime();
		sprintf(lcddata,"%02u:%02u:%02u   %01u",hourC,minC,secC,dayC);
		LcdSetCursor(4,0,"Time   Day");
		if ((hourC==tabtimes[0][0])&&(minC==tabtimes[0][1])&&(secC<5))
		{servoangle(3);
		}
		
		if ((hourC==tabtimes[1][0])&&(minC==tabtimes[1][1])&&(secC<5))
		{servoangle(4);
		}
		if ((hourC==tabtimes[2][0])&&(minC==tabtimes[2][1])&&(secC<5))
		{servoangle(5);
		}
		LcdSetCursor(2,1,lcddata);
		_delay_ms(200);
    }
}




void Readtime(){
	uint8_t bit[5];
	TWIStart();/* Start I2C with device write address */
	TWIWriteAddrs(104,0); //Return 24 mean device found, return mean no device
	TWIWriteData(0);  //Register address
	
	TWIStart();
	TWIWriteAddrs(104,1); //Write bit	
	bit[0]=TWIReadACK();
	bit[1]=TWIReadACK();
	bit[2]=TWIReadACK();
	bit[3]=TWIReadNACK();
	
	TWIStop();
	
	secC=(((bit[0]&0xF0)>>4)*10)+(bit[0]&0xF);
	minC=(((bit[1]&0xF0)>>4)*10)+(bit[1]&0xF);
	hourC=(((bit[2]&0b00110000)>>4)*10)+(bit[2]&0xF);
	dayC=bit[3]&0b111;
}

void servoangle(uint8_t pin){
	
	for(uint8_t j=0;j<100;j++){
		PORTD|=(1<<pin);
		for(uint8_t i=0;i<10;i++){
			_delay_us(100);
		}
		PORTD&=~(1<<pin);
		
		
		for(uint8_t i=0;i<10;i++){
			_delay_us(100);
		}
	}
	_delay_ms(500);
	for(uint8_t j=0;j<100;j++){
		
		PORTD|=(1<<pin);
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
		PORTD&=~(1<<pin);
		
			
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
	}
	
	
	
	
}

void setTime(uint8_t hSet,uint8_t mSet,uint8_t sSet){
	TWIStart();
	TWIWriteAddrs(104,0);
	TWIWriteData(0x02);
	TWIWriteData(((hSet/10)<<4)|(hSet%10));
	TWIStop();
	
	TWIStart();
	TWIWriteAddrs(104,0);
	TWIWriteData(0x01);
	TWIWriteData(((mSet/10)<<4)|(mSet%10));
	TWIStop();
	
	TWIStart();
	TWIWriteAddrs(104,0);
	TWIWriteData(0x00);
	TWIWriteData(((sSet/10)<<4)|(sSet%10));
	TWIStop();

	
}