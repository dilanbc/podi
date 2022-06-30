/*
 * c.cpp
 *
 * Created: 9/26/2021 12:58:11 AM
 * Author : 
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t i=0;
volatile bool dir=0;
int main(void)
{	DDRB=0b11111111;
	PORTD|=(0b11);
	TCCR1B|=(1<<WGM12);//ctc
	TCCR1B|=(1<<CS12); //prescaler=256
	TCNT1=0;
	OCR1A=31250;
	
	//
	PCICR|=(1<<PCIE2);
	PCMSK2|=(0b11);
	sei();
	
	while (1)
	{
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

ISR(PCINT2_vect){
	if (!(PIND&0b1))
	{TIMSK1^=(1<<OCIE1A);
	}
	if (!(PIND&0b10))
	{dir=~dir;
	}
}

