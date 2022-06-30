/*
 * pemal sine.cpp
 *
 * Created: 12/17/2021 11:59:29 PM
 * Author : dilan
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

volatile uint16_t sin1=0;

int main(void)
{
	/*PD6 pin part*/
	DDRD|=(1<<6);//pd6 output
	TCCR0A|=(1<<COM0A1);// from datasheet , non inverting mode
	TCCR0A|=(1<<WGM01)|(1<<WGM00);// from table , select fast pwm mode
	TCCR0B|=(1<<FOC0A);// force output
	TCCR0B|=(1<<CS00); // start timer , prescale=1;
	
	/*PD5 pin part*/
	DDRD|=(1<<5);//pd6 output
	TCCR0A|=(1<<COM0B1);// from datasheet , non inverting mode
	TCCR0B|=(1<<FOC0B);// force output
	
	/*PD6 pin part*/
	DDRB|=(1<<3);//pd6 output
	TCCR2A|=(1<<COM2A1);// from datasheet , non inverting mode
	TCCR2A|=(1<<WGM21)|(1<<WGM20);// from table , select fast pwm mode
	TCCR2B|=(1<<FOC2A);// force output
	TCCR2B|=(1<<CS20); // start timer , prescale=1;
	

	
	TCCR1B|=(1<<WGM12);//ctc
	TCCR1B|=(1<<CS10);//prescaler=1;
	TCNT1=0;
	OCR1A=444;
	TIMSK1|=(1<<OCIE1A);
	
	
	
	sei();
	
	
	while (1)
	{
	}
}

ISR(TIMER1_COMPA_vect){
	sin1++;
	if (360<=sin1)
	{sin1=0;
	}
	OCR0A=(((sin((2*M_PI/360)*sin1))*255)+255)/2;
	OCR0B=(((sin((2*M_PI/360)*(sin1+120)))*255)+255)/2;
	OCR2A=(((sin((2*M_PI/360)*(sin1+240)))*255)+255)/2;
	
}
