#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "USART.h"

char txdata[20];
volatile uint16_t TimerCal=0;// variable for collect echo data
uint16_t ultraINTL=0;
uint16_t ultraINTR=0;

void sendSMS(uint8_t sender,char*msg);

int main(void)
{	USART_Init(9600);
	
	
	
	/*Ultrasonic Timer Part*/	DDRA|=(1<<0); //D4 as output
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
		PORTA&=~(1<<0);//TRIG pin low
		_delay_us(50);//wait 50 micro sec
		PORTA|=(1<<0);//TRIG pin high
		_delay_us(50);//wait 50 micro sec
		PORTA&=~(1<<0);////TRIG pin low
		while(!(PINA&(1<<1)))//wait for pulse
		TimerCal=0;//rest timer
		while((PINA&(1<<1)))////wait for pulse down
		ultraINTL=TimerCal/4.15;//copy timer value
		_delay_ms(50);
		PORTA&=~(1<<0);//TRIG pin low
		_delay_us(50);//wait 50 micro sec
		PORTA|=(1<<0);//TRIG pin high
		_delay_us(50);//wait 50 micro sec
		PORTA&=~(1<<0);////TRIG pin low
		while(!(PINA&(1<<2)))//wait for pulse
		TimerCal=0;//rest timer
		while((PINA&(1<<2)))////wait for pulse down
		ultraINTR=TimerCal;//copy timer value
		
		sprintf(txdata,"length - %u",ultraINTL);
			
			USART_TxStringln("AT");
			_delay_ms(500);
			USART_TxStringln("AT");
			_delay_ms(500);
			USART_TxStringln("AT+CMGF=1");
			_delay_ms(500);
			USART_TxStringln("AT+CMGS=\"+94713673133\"");
			_delay_ms(500);
			USART_TxString(txdata);
			_delay_ms(500);
			USART_Transmit(26);
			_delay_ms(30000);
		
		
		
		_delay_ms(100);
    }
}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCal++;
	TCNT0=0;
	
}

