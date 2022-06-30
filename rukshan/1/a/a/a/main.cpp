/*
 * a.cpp
 *
 * Created: 9/25/2021 11:18:34 PM
 * Author : 
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t i=0;
int main(void)
{	DDRB=0b11111111;
    TCCR1B|=(1<<WGM12);//ctc
	TCCR1B|=(1<<CS12); //prescaler=256
	TCNT1=0;
	OCR1A=31250;
	TIMSK1|=(1<<OCIE1A);
	sei();
    while (1) 
    {
    }
}

ISR(TIMER1_COMPA_vect){
	TCNT1=0;
	i++;
	PORTB=i;
}
