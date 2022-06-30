#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "LCD.h"


uint16_t ultraINT=0;
volatile uint16_t TimerCal=0;// variable for collect echo data

char txdata[20];

int main(void)
{
    LcdInit(0x27);
	LcdSetCursor(0,0,"");
	
	DDRD|=(1<<2); //D2 as output
	TCCR0|=(1<<WGM01);//Enable Compare match mode
	TCCR0|=(1<<CS11);//Start timer  prescaler =8
	TCNT0=0;
	OCR0=10;
	/*register value= time*(clock speed/prescale)
	register value=0.000001*(8000000/1)
	register value=10*/
	TIMSK|=(1<<OCIE0);//enable timer Compare inturrept
	sei();
	
    while (1) 
    {
				PORTD&=~(1<<2);//TRIG pin low
				_delay_us(50);//wait 50 micro sec
				PORTD|=(1<<2);//TRIG pin high
				_delay_us(50);//wait 50 micro sec
				PORTD&=~(1<<2);////TRIG pin low
				while(!(PIND&(1<<3)))//wait for pulse
				TimerCal=0;//rest timer
				while((PIND&(1<<3)))////wait for pulse down
				ultraINT=TimerCal;//copy timer 
				ultraINT=ultraINT/4.15;
				sprintf(txdata,"length %03u",ultraINT);
				LcdSetCursor(0,0,txdata);
				_delay_ms(100);
		
    }
}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCal++;
	TCNT0=0;
	
}