#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "USART.h"


char txdata[20];
volatile uint16_t TimerCal=0;// variable for collect echo data
uint16_t ultraINT0=0;

int main(void)
{	USART_Init(9600);
	
	/*Ultrasonic Timer Part*/
	DDRD|=(1<<3); //D4 as output
	TCCR0|=(1<<WGM01);//Enable Compare match mode
	TCCR0|=(1<<CS11);//Start timer  prescaler =8
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
		TimerCal=0;//rest timer
		while((PIND&(1<<2)))////wait for pulse down
		ultraINT0=TimerCal/4.148148148;//copy timer value
		
		uint8_t pir=(PIND&(1<<6))>>6;
		
		sprintf(txdata," %u %u\r\n",ultraINT0,pir);
		USART_TxString(txdata);
		
		_delay_ms(100);
	
		
    }
}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCal++;
	TCNT0=0;
	
}
