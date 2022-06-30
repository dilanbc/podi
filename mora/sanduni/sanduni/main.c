#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include "LCD.h"

const uint8_t kaypad[4][4]={{7,8,9,10},{4,5,6,11},{1,2,3,12},{13,0,14,15}};
uint8_t Readkey();
uint8_t key;



int main(void)
{	DDRA|=0b111;//led as output
	LcdInit();
	LcdSetCursor(0,0,"Select Container");
	LcdSetCursor(0,1,"Press the key");
	_delay_ms(1000);
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"1-Rice 2-Noodles");
	LcdSetCursor(0,1,"3-Curry");
	
	while (1)
	{	
			key=Readkey();
			
			
				
			
			if ((key>3)||(key==0))
			{LcdCommand(LCD_CLEARDISPLAY);
				LcdSetCursor(0,0,"Error. Press ");
			LcdSetCursor(0,1,"Valid Number");
			_delay_ms(3000);
			LcdCommand(LCD_CLEARDISPLAY);
			LcdSetCursor(0,0,"1-Rice 2-Noodles");
			LcdSetCursor(0,1,"3-Curry");
			} 
			else
			{	
				LcdCommand(LCD_CLEARDISPLAY);
				switch(key){
				
				case 1:LcdSetCursor(0,0,"Rice Selected");break;
				case 2:LcdSetCursor(0,0,"Noodles Selected");break;
				case 3:LcdSetCursor(0,0,"Curry Selected");break;
				
			}
				break;
			}
		
		
	}
	
	while(1){
		PORTA|=(1<<key-1);
		_delay_ms(500);
		PORTA&=~(1<<key-1);
		_delay_ms(500);
	}
	
}

uint8_t Readkey(){
	DDRB=0b00001111;
	PORTB=0b11110000;
	uint8_t getx;
	uint8_t gety;
	while(1){
		
		uint8_t pin=(~(PINB|0x0F));
		if (pin)
		{
			
			
			switch(pin){
				
				
				case (1<<4) :getx=0; break;
				case (1<<5) :getx=1;break;
				case (1<<6) :getx=2;break;
				case (1<<7) :getx=3;break;
				
				
			} //switch
			DDRB=0b0;
			PORTB=0b0;
			
			DDRB=0b11110000;
			PORTB=0b00001111;
			
			pin=~(PINB|0xF0);
			
			switch(pin){
				case (1<<0) :gety=0;break;
				case (1<<1) :gety=1;break;
				case (1<<2) :gety=2;break;
				case (1<<3) :gety=3;break;
				
			} //switch
			
			DDRB=0b00001111;
			PORTB=0b11110000;
			
			
			return kaypad[gety][getx];
			
		}//if
		
		
		
	}//while 1
}
