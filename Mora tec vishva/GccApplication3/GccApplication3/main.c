#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "LCD.h"


char lcddata[20]; 
volatile uint8_t PeopleCounter=0;

int main(void)
{
	LcdInit();PORTD^=(1<<4);
	MCUCR|=(1<<ISC11)|(1<<ISC10)|(1<<ISC01)|(1<<ISC00);
	GICR|=(1<<INT0)|(1<<INT1);
	
	
	
	TCCR1B|=(1<<CS12)|(1<<CS10)|(1<<WGM12);//Start timer  prescaler =1024
	TCNT1=0;
	OCR1A=31250;
	
	
	
	DDRD|=(1<<4);
	DDRD|=(1<<5);
	
	sei();
	while (1)
	{
		
		
		sprintf(lcddata,"%03u u ",PeopleCounter);
		LcdSetCursor(5,0,lcddata);
		_delay_ms(10);
		if (PeopleCounter>10)
		{LcdSetCursor(0,1,"People limited  ");
		} 
		else
		{LcdSetCursor(0,1,"Plese Enter     ");
		}
		
	}
}
ISR(INT0_vect){
	PORTD^=(1<<4);
	if (TIMSK&(1<<OCIE1A))
	{TIMSK&=~(1<<OCIE1A);
		if (TCNT1>0)
		{PeopleCounter++;
			TCNT1=0;
		}
	} 
	else
	{TIMSK|=(1<<OCIE1A);
	}
	
	
}
ISR(INT1_vect){
	
	PORTD^=(1<<4);
	if (TIMSK&(1<<OCIE1A))
	{TIMSK&=~(1<<OCIE1A);
		if (TCNT1>0)
		{
			if (PeopleCounter>0)
			{PeopleCounter--;
			}
		}
	}
	else
	{TIMSK|=(1<<OCIE1A);
	}
}
ISR(TIMER1_COMPA_vect){//ultrasonic
	
	PORTD^=(1<<5);
	TIMSK&=~(1<<OCIE1A);//enable timer Compare inturrept
	TCNT1=0;
}

