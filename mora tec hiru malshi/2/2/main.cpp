
#define F_CPU 8000000
#include <avr/io.h>
#include "LCDI2C.h"

bool countCn=0;
uint8_t count=0;


int main(void)
{
	PORTD|=(1<<3);//pull up pin
	//PORTC|=(1<<2); // pull up mode pin
	LcdInit();
	LcdSetCursor(0,0,"Voice cont. mode");
	while (1)
	{
		
		
		if ((PINC&(1<<2)))
		{LcdSetCursor(0,0,"Voice cont. mode");
		}
		else if((PINC&(1<<3)))
		{LcdSetCursor(0,0," Joystick mode  ");
		}
		
		
	}
}

