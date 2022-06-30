#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"



int main(void)
{
	LcdInit();
	LcdSetCursor(0,0,"Test");
	LcdSetCursor(0,1,"ABCD");
	DDRD|=(1<<2);
	while (1)
	{
		PORTD|=(1<<2);
		_delay_ms(500);
		PORTD&=~(1<<2);
		_delay_ms(500);
		
		
		
	}
}
