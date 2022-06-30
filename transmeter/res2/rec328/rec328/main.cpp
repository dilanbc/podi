/*
 * rec328.cpp
 *
 * Created: 2/9/2021 10:20:14 PM
 * Author : Dilan
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"

#define SLEDPORT PORTD
#define SRED 6
#define SGRN 5

#define LLEDPORT PORTC
#define LRED 0
#define LGRN 1
#define LBLU 2

uint8_t waittemp=0;
uint16_t btry;

void InitADC()
{
	// Select 2.56
	ADMUX |= (1<<REFS0)|(1<<REFS1);
	//set prescaller to 128 and enable ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}
uint16_t ReadADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	return ADC;
}

int main(void)
{
	/*set pin*/
	DDRC|=(1<<5);
	PORTC&=~(1<<5);
	
	/*largeLed*/
	DDRC|=(1<<0)|(1<<1)|(1<<2);
		
	/*smallLed*/
	DDRD|=(1<<SRED)|(1<<SGRN);
	USART_Init(2400);
	
	SLEDPORT|=(1<<SRED);
	_delay_ms(100);
	SLEDPORT|=(1<<SGRN);
	_delay_ms(300);
	SLEDPORT&=~(1<<SRED);
	
	
	USART_ClearRX();
	USART_TxStringln("AT");
	_delay_ms(200);
	
	/*start setting*/
/*
	USART_ClearRX();
	USART_TxStringln("AT+C040");
	USART_ReciveWait("OK",500);
	_delay_ms(100);
//1st C050,2nd C060, 3rd C040
	
	USART_ClearRX();
	USART_TxStringln("AT+B2400");
	USART_ReciveWait("OK",500);
	_delay_ms(100);*/
	
	
	
	/*end setting*/
	
	USART_TxStringln("AT");
	_delay_ms(200);
	if (USART_ReciveWait("OK",1000,1))
	{
	}
	else
	{SLEDPORT&=~(1<<SGRN);
		_delay_ms(500);
		SLEDPORT|=(1<<SGRN);
	}
	
	
	
	
	PORTC|=(1<<5);
	DDRB&=~(1<<5); // off ligh pin
	PORTB|=(1<<5);
    InitADC();
    while (1) 
    {
		if (USART_ReciveWait("T3",100))
		{	_delay_ms(5);
			char*filtereN;
			strncpy(filtereN,strstr(rxvalue,"T")+2,1);
			int num= strcmp(filtereN,"B");
			if ((num==0)&&(((PINB&(1<<5))==0)))
			{LLEDPORT|=(1<<LBLU);
			LLEDPORT&=~(1<<LRED);
			LLEDPORT&=~(1<<LGRN);
			}
			else if (num==0)
			{LLEDPORT&=~(1<<LGRN);
			LLEDPORT&=~(1<<LBLU);
			LLEDPORT&=~(1<<LRED);
			}
			
			if (num==5)
			{LLEDPORT|=(1<<LGRN);
			LLEDPORT&=~(1<<LRED);
			LLEDPORT&=~(1<<LBLU);
			}
			if (num==16)
			{LLEDPORT|=(1<<LRED);
			LLEDPORT&=~(1<<LGRN);
			LLEDPORT&=~(1<<LBLU);
			}
			
			USART_ClearRX();
			waittemp=0;	
		
		}
		else{
		waittemp++;
		if (waittemp==3)
		{LLEDPORT&=~(1<<LGRN);
		LLEDPORT&=~(1<<LBLU);
		LLEDPORT&=~(1<<LRED);
		}
		if (waittemp>3)
		{waittemp=3;
		}
		}
		
		if (((PINB&(1<<5))==0))//if (((PINB&(1<<5))==0))
		{
			
			
			btry=ReadADC(3);
			
			if (ReadADC(3)>945)
			{SLEDPORT|=(1<<SGRN);SLEDPORT&=~(1<<SRED);
			}
			else
			{SLEDPORT|=(1<<SRED);SLEDPORT&=~(1<<SGRN);
			}
		} 
		else
		{SLEDPORT&=~(1<<SRED);SLEDPORT&=~(1<<SGRN);
		}
		
		
		
		
		
    }
}
