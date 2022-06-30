#define F_CPU 8000000
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "USART.h"

uint8_t pos=0;

int main(void)
{
    DDRD|=(0b111<<3);
	USART_Init(2400);
    while (1) 
    {
		if ((rchar>62)&&(rchar<68))
		{
			PORTD=(PORTD&0b11000111)|(1<<(rchar-62));
			eeprom_write_byte((uint8_t*)pos,rchar-65);
			pos++;
			rchar=0;
			_delay_ms(1000);
		}
		_delay_ms(1000);
    }
}

