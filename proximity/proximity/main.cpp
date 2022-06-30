#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "USART.h"

static volatile int pulse = 0;
static volatile int i = 0;

int main(void)
{	char show_a[16];
	int16_t count_a = 0;
	DDRD = 0b11111011;
	USART_Init(9600);
	USART_TxString("started");
	_delay_ms(50);

	GICR |= 1<<INT0;
	MCUCR |= 1<<ISC00;

	sei();
	
	while(1)
	{
		PORTD |= 1<<PIND6;
		_delay_us(15);

		PORTD &= ~(1<<PIND6);
		count_a = pulse/58;
		
		sprintf(show_a,"%i \r\n",count_a);
		USART_TxString(show_a);
		_delay_ms(100);
		
	}
}

ISR(INT0_vect)
{
	if(i == 0)
	{
		TCCR1B |= 1<<CS10;
		i = 1;
	}
	else
	{
		TCCR1B = 0;
		pulse = TCNT1;
		TCNT1 = 0;
		i = 0;
	}
}