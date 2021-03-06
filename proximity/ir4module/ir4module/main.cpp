/*
 * ir4module.cpp
 *
 * Created: 9/1/2021 12:52:19 AM
 * Author : dilan
 */ 


#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"


volatile uint16_t timeri=0;
volatile uint16_t timerj=0;
volatile uint8_t pcCounter=0;
volatile uint8_t delayCounter=0;
volatile uint8_t echoState=0;
volatile uint8_t echoOnState=0;

int main(void)
{	DDRB|=(0b11<<6)|(1<<0);
	DDRD|=(0b111<<5)|(1<<2);
	USART_Init(9600);
	
    TCCR0A|=(1<<WGM01);
	TCCR0B|=(1<<CS00);
	TCNT0=0;
	OCR0A=8;
	TIMSK0|=(1<<OCF0A);
	
	TCCR2A|=(1<<WGM21);
	TCCR2B|=(1<<CS21)|(1<<CS20)|(1<<CS22);
	TCNT2=0;
	OCR2A=156;
	TIMSK2|=(1<<OCF2A);
	
	PCICR|=(1<<PCIE1);
	PCMSK1=(1<<pcCounter);
	sei();
	
    while (1) 
    {
		
		
		
		/*pulse*/
		
		_delay_ms(10);
		PORTD|=(1<<2);
		_delay_us(5);
		PORTD&=~(1<<2);
		timeri=0;
		
		if (timerj<300)
		{
			
			echoState|=(1<<pcCounter);
			echoOnState|=(1<<pcCounter);
		
		
		}
		else if (timerj<700)
		{echoState&=~(1<<pcCounter);
			echoOnState|=(1<<pcCounter);
		}
		else{
			echoOnState&=~(1<<pcCounter);
		}
		pcCounter++;
		if (pcCounter>5)
		{pcCounter=0;
		}
		PCMSK1=(1<<pcCounter);
		
		/*USART_TxNumber(pcCounter);
		USART_TxString("  ");
		USART_TxNumber(timerj);
		USART_TxStringln("");*/
		
		
		
		
		
		
    }
}

ISR(TIMER0_COMPA_vect){
	timeri++;
	TCNT0=0;
}
ISR(TIMER2_COMPA_vect){//20ms
	delayCounter++;
	
	
	if (delayCounter>10)
	{delayCounter=0;
		if ((!(echoState&0b1))&&(echoOnState&0b1)) PORTD^=(1<<7);else PORTD&=~(1<<7);
		if ((!(echoState&0b10))&&(echoOnState&0b10)) PORTD^=(1<<6);else PORTD&=~(1<<6);
		if ((!(echoState&0b100))&&(echoOnState&0b100)) PORTD^=(1<<5);else PORTD&=~(1<<5);
		if ((!(echoState&0b1000))&&(echoOnState&0b1000)) PORTB^=(1<<7);else PORTB&=~(1<<7);
		if ((!(echoState&0b110000))&&(echoOnState&0b110000)) PORTB^=(1<<6);else PORTB&=~(1<<6);
		if ((!(echoState&0b111111))&&(echoOnState&0b111111)) PORTB^=(1<<0);else PORTB&=~(1<<0);
		
	}
	else{
		if ((echoState&0b1)&&(echoOnState&0b1)) PORTD^=(1<<7); else PORTD&=~(1<<7);
		if ((echoState&0b10)&&(echoOnState&0b10)) PORTD^=(1<<6);else PORTD&=~(1<<6);
		if ((echoState&0b100)&&(echoOnState&0b100)) PORTD^=(1<<5);else PORTD&=~(1<<5);
		if ((echoState&0b1000)&&(echoOnState&0b1000)) PORTB^=(1<<7);else PORTB&=~(1<<7);
		if ((echoState&0b110000)&&(echoOnState&0b110000)) PORTB^=(1<<6);else PORTB&=~(1<<6);
		if ((echoState&0b111111)&&(echoOnState&0b111111)) PORTB^=(1<<0);else PORTB&=~(1<<0);
	}
}
ISR(PCINT1_vect){
	timerj=timeri;		timeri=0;
	
	
}
