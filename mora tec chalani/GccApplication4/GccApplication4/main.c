#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"



int main(void)
{
	USART_Init(9600);
	_delay_ms(500);
	USART_TxStringln("AT");
	_delay_ms(500);
	USART_TxStringln("AT");
	_delay_ms(500);
	USART_TxStringln("AT+CMGF=1");
	

	
	
	while (1)
	{
		
		
		if (PINB&(1<<3))
		{
			USART_Init(9600);
			_delay_ms(500);
			USART_TxStringln("AT");
			_delay_ms(500);
			USART_TxStringln("AT");
			_delay_ms(500);
			USART_TxStringln("AT+CMGF=1");
			_delay_ms(500);
			USART_TxStringln("AT+CMGS=\"+94711111111\"");
			_delay_ms(200);
			USART_TxString("sample text");
			_delay_ms(200);
			USART_Transmit(26);
			_delay_ms(200);
			_delay_ms(30000);
		}
		
		
		
		
	}
}



