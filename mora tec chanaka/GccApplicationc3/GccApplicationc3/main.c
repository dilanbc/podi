#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD.h"

char lcddata[20];

int main(void)
{
	LcdInit();
	DDRD|=(0b11<<4);//heater and water in output
	DDRA|=(1<<7);//water out output
	
	PORTD|=(1<<5);//heater on
	uint8_t i=0; //counter
	
	
	while (1)
	{
		sprintf(lcddata," time %02uS",i/2);
		LcdSetCursor(0,0,lcddata);
		
		
		if (PIND&(1<<0))// if logic==1
		{
			PORTD&=~(1<<5);//heater off
			PORTD|=(1<<4);//water in
			
		}
		
		else
		{PORTD&=~(1<<4);//water in off
			PORTD|=(1<<5);//heater on
			i++;
		
			if (i>40)
			{break;
			}
		}
		_delay_ms(500);
		
	}
	PORTD&=~(1<<5);//heater off
	PORTD&=~(1<<4);//water in off
	PORTA|=(1<<7);//water out on

}

