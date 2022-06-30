/*
 * pnu.cpp
 *
 * Created: 10/20/2020 12:12:43 AM
 * Author : Dilan
 */ 


#define show 4
#define powerdown 2

#define input 3

#define r0 0  //relay 0
#define r1 1  //relay 1

#define F_CPU 1200000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

volatile uint8_t State =0;


bool inputRead() {return PINB&(1<<input);}
bool powerDowntRead() {return PINB&(1<<powerdown);}
void setState();
int main(void)
{
	_delay_ms(500);
	
	
	State =eeprom_read_byte((uint8_t*)12);
	switch(State){
		case 1: if (inputRead())
		{ State=3;
		}
		else{State=2;}
		 break;
		 
		case 3: if (inputRead())
		{ State=1;
		}
		else{State=0;}
		break;
		
		case 0:if (inputRead())
		{State=1;
		}
		break;
		
		case 2:if (inputRead())
		{State=3;
		}
		 
	}
	
	DDRB|=(1<<show)|(1<<r1)|(1<<r0);
	for(uint8_t i=0;i<State+1;i++){
		PORTB|=(1<<show);
		_delay_ms(100);
		PORTB&=~(1<<show);
		_delay_ms(100);
		
	}
	
	
	GIMSK|=(1<<PCIE); //pin change inttupet enable
	DDRB&=~(1<<input);//define input pin
	PCMSK|=(1<<input);  // select input pin as pin change
    if (inputRead())
    {setState();
    }
	
	DDRB&=~(1<<powerdown); //define power down
	ADMUX&=~(1<<REFS0);
	ADMUX|=(1<<MUX0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADIE);

	
	ADCSRA |= (1<<ADSC); //start
	
	
	
	
	
	
	
	
   
   sei();
    while (1) 
    {
		
		
    }
}
void setState(){
	if (State==0)
	{PORTB&=~(1<<r0);
		PORTB&=~(1<<r1);
	}
	if (State==1)
	{PORTB|=(1<<r0);
		PORTB&=~(1<<r1);
	}
	if (State==2)
	{PORTB&=~(1<<r0);
		PORTB&=~(1<<r1);
	}
	if (State==3)
	{PORTB&=~(1<<r0);
		PORTB|=(1<<r1);
	}
	
}


ISR (PCINT0_vect){
	if (inputRead()!=State%2)
	{PORTB|=(1<<show);
		State++;
		if (State>3)
		{State=0;
		}
		setState();
		_delay_ms(100);
		
		PORTB&=~(1<<show);
		
	}
	
	
}

ISR(ADC_vect){
	if (ADC<500){
		
		eeprom_write_byte((uint8_t*)12,State);
		for(uint8_t i=0;i<State+1;i++){
			PORTB|=(1<<show);
			_delay_ms(100);
			PORTB&=~(1<<show);
			_delay_ms(100);
			
		}PORTB|=(1<<show);
		_delay_ms(2000);
		
	}
	
	ADCSRA |= (1<<ADSC); //start
}