/*
 * 3.cpp
 *
 * Created: 9/26/2021 9:38:09 PM
 * Author : 
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t i=0;
volatile bool push=0;
volatile uint8_t segNum=0;
volatile bool segPos=0;
int main(void)
{	DDRB=0b11111111;
	DDRC|=(0b11111);
	PORTD|=(1<<0);
	
	TCCR1B|=(1<<WGM12);//ctc
	TCCR1B|=(1<<CS12); //prescaler=256
	TCNT1=0;
	OCR1A=31250;
	TIMSK1|=(1<<OCIE1A);
	//
	TCCR0A|=(1<<WGM01);
	TCCR0B|=(0b11);//64
	TCNT0=0;
	OCR0A=125;
	TIMSK0|=(1<<OCIE0A);
	sei();
	while (1)
	{if ((!push)&&(!(PIND&1)))
	{push=1;
	}
	}
}

ISR(TIMER1_COMPA_vect){
	TCNT1=0;
	if (push)
	{i++;
	
	if ((i)&&(i<6))
	{segNum=0;
		PORTC=(PORTC&0b00000011)|0b001000;
		
	}
	else if ((i)&&(i<16))
	{segNum=15-i;
		PORTC=(PORTC&0b00000011)|0b000100;
	}
	else 
	{segNum=0;push=0;i=0;
		PORTC=(PORTC&0b00000011)|0b010000;
	}
	}
	else {
	segNum=0;push=0;i=0;
	PORTC=(PORTC&0b00000011)|0b010000;	
	}
	//PORTB=i;
	//PORTB=0b110;//1
	//PORTB=0b1011011;//3
	//PORTB=0b1100110;//4
	//PORTB=0b1101101;//5
	//PORTB=0b1111101;//6
	//PORTB=0b111;//7
	//PORTB=0b1111111;//8
	//PORTB=0b1100111;//9
	//PORTB=0b111111;//0
	
}
ISR(TIMER0_COMPA_vect){
	TCNT0=0;
	
	if (segPos)
	{PORTC=(PORTC&0b11111100)|0b01;
		switch(segNum%10){
			case 0:PORTB=0b111111;break;
			case 1:PORTB=0b110;break;
			case 2:PORTB=0b1011011;break;
			case 3:PORTB=0b1001111;break;
			case 4:PORTB=0b1100110;break;
			case 5:PORTB=0b1101101;break;
			case 6:PORTB=0b1111101;break;
			case 7:PORTB=0b111;break;
			case 8:PORTB=0b1111111;break;
			case 9:PORTB=0b1100111;break;
		}
	}
	else
	{PORTC=(PORTC&0b11111100)|0b10;
		switch(segNum/10){
			case 0:PORTB=0b111111;break;
			case 1:PORTB=0b110;break;
			case 2:PORTB=0b1011011;break;
			case 3:PORTB=0b1001111;break;
			case 4:PORTB=0b1100110;break;
			case 5:PORTB=0b1101101;break;
			case 6:PORTB=0b1111101;break;
			case 7:PORTB=0b111;break;
			case 8:PORTB=0b1111111;break;
			case 9:PORTB=0b1100111;break;
		}
	}
	segPos=!segPos;
}
