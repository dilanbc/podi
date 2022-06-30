#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include "LCD.h"

const uint8_t kaypad[4][4]={{7,8,9,10},{4,5,6,11},{1,2,3,12},{13,0,14,15}};
uint8_t getkeyNum();
char lcddata[20];

uint8_t hx711H=0; //Load Scale High Bits
uint16_t hx711L=0;//Load Scale Low Bits
volatile uint16_t TimerCal=0;// variable for collect echo data
float loadCellRead();

uint8_t key;

int main(void)
{
	LcdInit();
	bool state=PINA&0b1;
	
	DDRC|=(1<<0); //Load cell clock pin
	PORTC&=~(1<<0);//Clock pin low
	
    while (1) 
    {	if (state)
    {
		key=getkeyNum();
		sprintf(lcddata,"%02u",key);
		LcdSetCursor(0,0,lcddata);
    } 
    else
    {
		float hx=loadCellRead();
		sprintf(lcddata," %.3f  ",hx);
		LcdSetCursor(0,0,lcddata);
		_delay_ms(100);
    }
		
		
    }
}

uint8_t getkeyNum(){
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

float loadCellRead(){
	hx711H=0;hx711L=0;  //clear variables
	for(uint8_t i=0;i<8;i++){  // Load cell data high 8 bits
		PORTC|=(1<<0); //Clock pin high
		_delay_us(10);
		if ((PINC&(1<<1))>>1)  //read data pin
		{hx711H|=(1<<(7-i));//set hx 711 varible
		}
		else
		{hx711H&=~(1<<(7-i));
		}
		PORTC&=~(1<<0); //Clock pin low
		_delay_us(5);
	}
	
	
	for(uint8_t i=0;i<16;i++){ // Load cell data low 16 bits
		PORTC|=(1<<0); //Clock pin high
		_delay_us(10);
		if ((PINC&(1<<1))>>1) //read data pin
		{hx711L|=(1<<(15-i));
		}
		else
		{hx711L&=~(1<<(15-i));
		}
		PORTC&=~(1<<0); //Clock pin low
		_delay_us(5);
	}
	
	hx711L=hx711L>>1; //shift bits
	
	if (hx711H&1)  //bit setup
	{hx711L|=(1<<15);
	}
	else
	{hx711L&=~(1<<15);
	}
	hx711H=hx711H>>1;
	
	return (hx711H*(65536/18029.6))+hx711L/18029.6; //load cell calibration
}