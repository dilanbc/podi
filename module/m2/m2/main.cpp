
#include <avr/io.h>
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>  //library for sprintlf function
#include "USART.h"
#include "LCDI2C.h"

#define LENGTH 82 //20
volatile uint16_t TimerCal=0;
uint16_t ultra=0;
 char txword[15];

int main(void)
{
   /*Ultrasonic Timer Part*/
	 DDRD|=(1<<4); //D2 as output
	TCCR0|=(1<<WGM01);//Enable Compare match mode
	TCCR0|=(1<<CS11);//Start timer  prescaler =8
	TCNT0=0;
	OCR0=10;
	/*register value= time*(clock speed/prescale)
	register value=0.00001*(8000000/1)
	register value=80*/
	TIMSK|=(1<<OCIE0);//enable timer Compare inturrept
	sei();
	
	DDRD|=(0b11<<6);//relay
	USART_Init(9600);  //Start Serial Communication
	LcdInit(); //Start LCD
	LcdSetCursor(0,0,"Wash your hands");
    while (1) 
    {
		/*Ultrasonic Part*/
		PORTD&=~(1<<4);
		_delay_us(50);
		PORTD|=(1<<4);
		_delay_us(50);
		PORTD&=~(1<<4);
		while(!(PIND&(1<<5)))
		TimerCal=0;
		while((PIND&(1<<5)))
		ultra=TimerCal;
		
		if (ultra<41)//10cm
		{	PORTD|=(0b11<<6);//on relay
			LcdSetCursor(0,1,"   Cleaning ...");
			_delay_ms(2000);
		LcdSetCursor(0,1,"               ");
		PORTD&=~(0b11<<6);	//off relay
		}
		
		sprintf(txword," %u \r\n ",ultra);
		USART_TxStringln(txword);
		_delay_ms(100);
    }
}

ISR(TIMER0_COMP_vect){
	TimerCal++;
	TCNT0=0;
	
}