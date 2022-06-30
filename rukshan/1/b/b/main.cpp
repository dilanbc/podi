/*
 * b.cpp
 *
 * Created: 9/25/2021 11:36:00 PM
 * Author : 
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t i=0;
volatile bool dir=0;
volatile bool mainb=0;
volatile bool dirb=0;

int main(void)
{	DDRB=0b11111111;
	PORTD|=(0b11);
	TCCR1B|=(1<<WGM12);//ctc
	TCCR1B|=(1<<CS12); //prescaler=256
	TCNT1=0;
	OCR1A=31250;
	
	sei();
	
	
	while (1)
	{
		if ((!(PIND&0b1))&&(!mainb))
		{TIMSK1^=(1<<OCIE1A);
		mainb=1;
		}
		if (PIND&0b1)
		{mainb=0;
		}
		
		 if ((!(PIND&0b10))&&(!dirb))
		{dir=~dir;
		dirb=1;
		}
		if (PIND&0b10)
		{dirb=0;
		}
		
		
	}
}

ISR(TIMER1_COMPA_vect){
	TCNT1=0;
	if (!dir)
	{i++;
	} 
	else
	{i--;
	}
	
	PORTB=i;
}