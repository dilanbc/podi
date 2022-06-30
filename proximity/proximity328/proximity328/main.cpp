/*
 * proximity328.cpp
 *
 * Created: 8/22/2021 10:27:50 PM
 * Author : dilan
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"

volatile uint16_t timeri=0;
volatile uint16_t timerj=0;
int main(void)
{	USART_Init(9600);
	DDRB|=(1<<5);
	DDRB|=(1<<0);
    TCCR0A|=(1<<WGM01);
	TCCR0B|=(1<<CS00);
	OCR0A=8;
	TIMSK0|=(1<<OCF0A);
	
	PCICR|=(1<<PCIE0);
	PCMSK0|=(1<<PCINT1);
	sei();
	
    while (1) 
    {
		/*echo pin PINB1 (15) arduino digital pin 9
		trig pin PINB0 (14)	arduino digital pin 8
		led pin  PINB5 (19) arduino digital pin 13*/
		PORTB&=~(1<<0);
		_delay_us(5);
		
		PORTB|=(1<<0);
		_delay_us(10);
		PORTB&=~(1<<0);
		timeri=0;
		if (timerj<500)
		{PORTB|=(1<<5);
		}
		else{
			PORTB&=~(1<<5);
		}
		USART_TxNumber(timerj);
		USART_TxStringln("");
		_delay_ms(1);
		timeri=0;
		
		
		
		
    }
}

ISR(TIMER0_COMPA_vect){
	timeri++;
}
ISR(PCINT0_vect){
	timerj=timeri;		timeri=0;
	
	
}