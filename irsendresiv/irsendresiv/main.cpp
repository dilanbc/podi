/*
 * irsendresiv.cpp
 *
 * Created: 8/10/2021 8:32:16 PM
 * Author : dilan
 */ 
#define F_CPU 9600000
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    DDRB|=(1<<3);//ir trns
	DDRB&=~(1<<4);
   while(1)
   {PORTB|=(1<<3);//ir trns
		_delay_ms(2000);
		PORTB&=~(1<<3);//ir trns
		_delay_ms(2000);
    }
}

