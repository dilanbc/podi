/*
 * bicycle.cpp
 *
 * Created: 3/7/2021 8:13:38 PM
 * Author : Dilan
 */ 
#define F_CPU 1200000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

bool jmper=0;
uint8_t lgcount=0;
uint8_t timercount=0;

#define timerlogic 1.3*10


void timerInit(){
	TCCR0A|=(1<<WGM01);
	TCCR0B|=(1<<CS02)|(1<<CS00);
	OCR0A=117;
	TIMSK0|=(1<<OCIE0A);
}

ISR(TIM0_COMPA_vect){
if (lgcount>0)
{timercount++;
	if (timercount>12)
	{timercount=0;
		lgcount=0;
	} 
	else
	{if (lgcount>1)
	{PORTB|=(1<<0);
	}
	}
}

}

void sensorInit(){
DDRB&=~(1<<3);//sensor
PORTB|=(1<<3);//pullup
//hole effect normal=1 magnet=0;
GIMSK|=(1<<PCIE);
PCMSK|=(1<<PCINT3);
	
}

ISR(PCINT0_vect){
if (jmper)
{
	if (((PINB&0b1000)>>3)==0)
	{lgcount++;
	}
} 
else
{
	if (((PINB&0b1000)>>3)!=0)
	{lgcount++;
	}
}
}

int main(void)
{	
    DDRB|=(1<<0);//relay
	PORTB|=(1<<4);//pullup
	
	
	
	
	
	
	if (((PINB&0b10000)>>4)==0)
	{jmper=1;
	}
	sensorInit();
	timerInit();
	sei();
    while (1) 
    {
		
    }
}

