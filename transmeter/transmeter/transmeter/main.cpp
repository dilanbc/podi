/*
 * transmeter.cpp
 *
 * Created: 11/29/2020 9:24:57 AM
 * Author : Dilan
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
//#include "SoftUART.h"

#define REDL 0 
#define GRNL 1
#define BLUL 2




int main(void)
{
    USART_Init(2400);
	//SoftUART_Init();
	/*set pin*/
	DDRC|=(1<<5);
	PORTC&=~(1<<5);
	
	/*led*/
	DDRC|=(1<<0)|(1<<1)|(1<<2);
	PORTC|=(1<<BLUL);
	
	/*stereo*/
	DDRD&=~(1<<2);
	DDRD&=~(1<<3);
	
	_delay_ms(500);
	USART_ClearRX();
	//SoftUSART_tx_strln("Started");
	USART_TxStringln("AT");
	_delay_ms(500);
	
	USART_ClearRX();
	//SoftUSART_tx_strln("1st");
	USART_TxStringln("AT");
	USART_ReciveWait("OK",500);
	_delay_ms(100);
	//SoftUSART_tx_strln(rxvalue);
	_delay_ms(500);
	
	/*setting*/
	/*USART_ClearRX();
	SoftUSART_tx_strln("1st");
	USART_TxStringln("AT+C060");
	USART_ReciveWait("OK",500);
	_delay_ms(100);
	//SoftUSART_tx_strln(rxvalue);
	_delay_ms(500);*/
	
	/*USART_ClearRX();
	SoftUSART_tx_strln("1st");
	USART_TxStringln("AT+B2400");
	USART_ReciveWait("OK",500);
	_delay_ms(100);
	//SoftUSART_tx_strln(rxvalue);
	_delay_ms(500);*/
	
	/*end setting*/
	
	USART_ClearRX();
	//SoftUSART_tx_strln("2nd");
	USART_TxStringln("AT+P8");
	USART_ReciveWait("OK",500);
	_delay_ms(100);
	//SoftUSART_tx_strln(rxvalue);
	_delay_ms(500);
	
	
	PORTC|=(1<<5);
	
    while (1) 
    {	
		
		
		if (PIND&(1<<2))
		{PORTC|=(1<<GRNL); PORTC&=~(1<<BLUL);PORTC&=~(1<<REDL);
		USART_TxString("T2GE");	
		}

		else if (PIND&(1<<3))
		{PORTC|=(1<<REDL); PORTC&=~(1<<BLUL);PORTC&=~(1<<GRNL);
			USART_TxString("T2RE");
		}
		else
		{PORTC|=(1<<BLUL); PORTC&=~(1<<GRNL);PORTC&=~(1<<REDL);
			USART_TxString("T2BE");
		}
		_delay_ms(50);
    }
}

