#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>


char txdata[20];
volatile uint16_t TimerCalVal=0;// variable for collect echo data
uint16_t ultralength=0;

int main(void)
{	
	
	/*Ultrasonic Timer Part*/
	DDRD|=(1<<3); //D4 as output
	TCCR0|=(1<<WGM01);//Enable Compare match mode
	TCCR0|=(1<<CS01);//Start timer  prescaler =8
	TCNT0=0;
	OCR0=10;
	/*register value= time*(clock speed/prescale)
	register value=0.000001*(8000000/1)
	register value=10*/
	TIMSK|=(1<<OCIE0);//enable timer Compare inturrept
	sei();
	DDRD|=(1<<4);
    while (1) 
    {
		PORTD&=~(1<<3);//TRIG pin low
		_delay_us(50);//wait 50 micro sec
		PORTD|=(1<<3);//TRIG pin high
		_delay_us(50);//wait 50 micro sec
		PORTD&=~(1<<3);////TRIG pin low
		while(!(PIND&(1<<2)))//wait for pulse
		TimerCalVal=0;//rest timer
		while((PIND&(1<<2)))////wait for pulse down
		ultralength=TimerCalVal/4.1282;//copy timer value
		
		
		_delay_ms(100);
		
		if (ultralength<50)
		{PORTD|=(1<<4);
		
		} 
		else
		{PORTD&=~(1<<4);
		}
		
    }
}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCalVal++;
	TCNT0=0;
	
}
