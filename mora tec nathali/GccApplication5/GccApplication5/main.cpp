#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD.h"

const uint8_t kaypad[4][4]={{7,8,9,10},{4,5,6,11},{1,2,3,12},{13,0,14,15}};
uint8_t getkeyNum();
char lcddata[20];
uint8_t keyout;

int main(void)
{	
	LcdInit();
	LcdSetCursor(0,0,"");
     while (1) 
    {
		keyout=getkeyNum();
	//	sprintf(lcddata,"%02u",keyout);
		LcdChar(keyout+48);
    }
}

uint8_t getkeyNum(){
	DDRA=0b00001111;
	PORTA=0b11110000;
	uint8_t getx;
	uint8_t gety;
	while(1){
		
		uint8_t pin=(~(PINA|0x0F));
		if (pin)
		{
			
			
			switch(pin){
				
				
				case (1<<4) :getx=0; break;
				case (1<<5) :getx=1;break;
				case (1<<6) :getx=2;break;
				case (1<<7) :getx=3;break;
				
				
			} //switch
			DDRA=0b0;
			PORTA=0b0;
			
			DDRA=0b11110000;
			PORTA=0b00001111;
			
			pin=~(PINA|0xF0);
			
			switch(pin){
				case (1<<0) :gety=0;break;
				case (1<<1) :gety=1;break;
				case (1<<2) :gety=2;break;
				case (1<<3) :gety=3;break;
				
			} //switch
			
			DDRA=0b00001111;
			PORTA=0b11110000;
			
			
			return kaypad[gety][getx];
			
		}//if
		
		
		
	}//while 1
}
