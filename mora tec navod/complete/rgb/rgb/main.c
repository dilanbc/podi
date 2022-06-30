#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "USART.h"

char datast[15]={0};

uint16_t analog;
uint8_t last=0;

int main(void)
{
   USART_Init(9600);
   ADMUX|=(1<<REFS0);//reference select
   ADCSRA|=(1<<ADEN);//enable analog
   ADCSRA|=(1<<ADPS0);//prescaler=2
   
    while (1) 
    {
		if (PIND&0b1100)
		{
			uint8_t pin=((PIND&0b1100)>>2)-1;
			
			if (pin!=last)
			{ADMUX = (ADMUX & 0xF0) | (pin & 0x0F);
		
		ADCSRA |= (1<<ADSC);
		
		while( ADCSRA & (1<<ADSC) );//wait for 0
		analog= ADC;
		USART_Transmit((analog-1)/4);
		last=pin;
			}
		
		}
		
		
		
		
		
    }
}

