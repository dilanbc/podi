#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"

volatile uint16_t TimerCal=0;// variable for collect echo data
uint16_t ultraINTL=0;
	
int main(void)
{
    USART_Init(9600);
	_delay_ms(500);
	USART_TxStringln("AT");
	_delay_ms(500);
	USART_TxStringln("AT");
	_delay_ms(500);
	USART_TxStringln("AT+CMGF=1");
	
	/*Ultrasonic Timer Part*/
	DDRA|=(1<<0); //D4 as output
	TCCR0|=(1<<WGM01);//Enable Compare match mode
	TCCR0|=(1<<CS11);//Start timer  prescaler =8
	TCNT0=0;
	OCR0=10;
	/*register value= time*(clock speed/prescale)
	register value=0.000001*(8000000/1)
	register value=10*/
	TIMSK|=(1<<OCIE0);//enable timer Compare inturrept
	sei();
	
	DDRB|=(1<<0);
    while (1) 
    {
		
		PORTA&=~(1<<0);//TRIG pin low
		_delay_us(50);//wait 50 micro sec
		PORTA|=(1<<0);//TRIG pin high
		_delay_us(50);//wait 50 micro sec
		PORTA&=~(1<<0);////TRIG pin low
		while(!(PINA&(1<<1)))//wait for pulse
		TimerCal=0;//rest timer
		while((PINA&(1<<1)))////wait for pulse down
		ultraINTL=TimerCal;//copy timer value
		if (ultraINTL<200)
		{PORTB|=(1<<0);
		} 
		else
		{PORTB&=~(1<<0);
		}
		_delay_ms(50);
    }
}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCal++;
	TCNT0=0;
	
}


