/*
 * attiny84.cpp
 *
 * Created: 2/28/2022 6:55:05 PM
 * Author : dilan
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{	DDRB|=(1<<2)|(1<<0);
    USISR|=(1<<USIWM1);//2wire
	USISR|=(1<<USICLK);//2wire
	USISR|=(1<<USICNT3)|(1<<USICNT2)|(1<<USICNT1); //Set counter to 1110 to force it to overflow when ACK bit is received
	
    while (1) 
    {
		 USIDR=100;          //Writing data bytes in Data register
		 while(!(USISR&(1<<USIOIF)))         //Checking USI data counter overflow flag to detect the end of transmission every byte
		 {
			 USICR|=(1<<USICLK);  //Enabling clock for transmission and generating clock for slave device
		 }
		 USISR|=(1<<USIOIF);             //Clearing counter overflow flag
		 _delay_ms(100);
    }
}

