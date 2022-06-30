
#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "LCDI2C.h"

uint8_t count=0;
char lcd[16];

int main(void)
{
   LcdInit();
   
   GICR|=(1<<INT1)|(1<<INT0);//enable int1 and int 0
   MCUCR|=(1<<ISC11)|(1<<ISC01);// falling edge inturrept
   sei();//enable global int
   DDRA|=(1<<2);
    while (1) 
    {
		sprintf(lcd,"Count %u",count);
		LcdSetCursor(4,0,lcd);
		if (count<5)
		{LcdSetCursor(4,1,"OPEN  ");//lcd
			PORTA&=~(1<<2);
		} 
		else
		{LcdSetCursor(4,1,"CLOSED ");//lcd
			PORTA|=(1<<2);
		}
		
    }
}

ISR(INT0_vect){//in
count++;	
}

ISR(INT1_vect){//out
if (count>0)
{count--;
}
	
}
