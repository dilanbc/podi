#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "USART.h"
#include "LCD.h"

char lengthcal[20];//string memory
uint8_t checkS,R,G,B=0;
char lcddata[20];

int main(void)
{
    LcdInit();
	LcdSetCursor(0,0,"ww");
	USART_Init(9600);
	DDRD|=(0b11<<2);
	DDRC=0xFF;
    while (1) 
    {
	PORTD=(PORTD&0b11110011)|(0b1<<2);
	_delay_ms(50);
	R=rchar;
	
	PORTD=(PORTD&0b11110011)|(0b10<<2);
	_delay_ms(50);
	G=rchar;
	
	PORTD=(PORTD&0b11110011)|(0b11<<2);
	_delay_ms(50);
	B=rchar;
	PORTD=(PORTD&0b11110011);
	
	sprintf(lcddata,"%03u %03u %03u ",R,G,B);
	LcdSetCursor(0,0,lcddata);
	_delay_ms(500);
	
    }
}

