#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD.h"

uint8_t Pressure= 22;
uint8_t Tempurature=36;

char lcddata[20];
int main(void)
{
	LcdInit();
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Place Finger");
	PORTC|=(1<<2);
	
	
	while (1)
	{
		if (!(PINC&(1<<2)))
		{	LcdCommand(LCD_CLEARDISPLAY);
			break;
		}
	}
	
	sprintf(lcddata,"Tempura %02u",Tempurature);
	LcdSetCursor(0,0,lcddata);
	sprintf(lcddata,"Pressure %02u",Pressure);
	LcdSetCursor(0,1,lcddata);
	_delay_ms(1000);
}
