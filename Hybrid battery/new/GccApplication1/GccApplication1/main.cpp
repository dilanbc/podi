/*
 * GccApplication1.cpp
 *
 * Created: 11/20/2020 7:01:13 AM
 * Author : Dilan
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"

#define SPI_DDR DDRB
#define MOSI    3
#define MISO    4
#define SCK     5

#define CSPORT  PORTB
#define CSDDR	DDRB
#define CS      PINB1
volatile uint8_t shiftMemory[4]={};

void shiftwrite(uint8_t shiftvalue,bool state);
void shiftClear();


int main(void)
{	USART_Init(9600);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	SPI_DDR |=  (1 << MOSI) | (1 << SCK);
	SPI_DDR&=~(1<<MISO);
	CSDDR |=(1<<CS);
	CSPORT|=(1<<CS);
	
	SPI_DDR|=(1<<2);
	PORTB|=(1<<2);
	
	
	
	int j=-0;
	while (1)
	{	
		
		
		USART_TxStringln("E");
		shiftwrite(j,1);
		j++;
		if (j==9)
		{j=0;
			shiftClear();
		}
		_delay_ms(200);
		
		
	}
}

void shiftwrite(uint8_t shiftvalue,bool state){
	
	
	shiftvalue=shiftvalue+((shiftvalue-1)/7);  //Q0 pin remove
	
	if (state)
	{shiftMemory[shiftvalue/8]|=(1<<(shiftvalue%8));
	} 
	else
	{shiftMemory[shiftvalue/8]&=~(1<<(shiftvalue%8));
	}
	
	
	
	
	CSPORT&=~(1<<CS);	
	
	for (uint8_t i=0;i<4;i++)
	{SPDR = shiftMemory[3-i];
	while(!(SPSR & (1 << SPIF)));
	}
	CSPORT|=(1<<CS);

	
}

void shiftClear(){
	
	for(uint8_t i=0;i<4;i++){
		shiftMemory[i]=0;
	}
}