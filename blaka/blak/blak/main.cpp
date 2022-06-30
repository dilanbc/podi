/*
 * blak.cpp
 *
 * Created: 2/26/2021 2:33:24 PM
 * Author : Dilan
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <setjmp.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "USART.h"

char temptext[50];
uint16_t tempnum;
char numbr[30];
uint8_t blink=0; uint8_t tempblink=0;
volatile bool sent=1;
jmp_buf mainstart;
jmp_buf gsmstart;


void init_timer(){
	TCCR1B|=(1<<WGM12);
	TCCR1B|=(1<<CS12)|(1<<CS10); 
	//prescaler is 1024, so (1/8000000)/*1024=0.128*1000
	OCR1A=100/0.128;
	TIMSK1|=(1<<OCIE1A);
	DDRC|=(1<<0);
}

ISR(TIMER1_COMPA_vect){
PORTC&=~(1<<0);
tempblink++;
if (tempblink>blink)
{tempblink=0;
	PORTC|=(1<<0);
}


}

void GSM_init(){
	USART_TxStringln("AT");
	_delay_ms(200);
	setjmp (gsmstart);
	uint8_t gsmstate=0;
	USART_ClearRX();
	USART_TxStringln("AT+CREG?");
	USART_ReciveWait("+CREG:",2000);
	_delay_ms(100);
	strncpy(temptext,strstr(rxvalue,"+CREG:")+9,1);
	gsmstate=atoi(temptext);
	tempnum=gsmstate;
	
	
	if (gsmstate==5)
	{blink=30;
	} 
	else if(gsmstate==2)
	{blink=5;
		_delay_ms(1000);
		longjmp(gsmstart,1);
		
	}
	else{
		blink=1;
		_delay_ms(2000);
		USART_TxStringln("AT");
		_delay_ms(200);
		longjmp(mainstart,1);
		
	}
	
	_delay_ms(500);
	init_timer();
}

void smsInit (){
_delay_ms(200);
USART_TxStringln("AT+CMGF=1");
if (USART_ReciveWait("OK",1000))
{USART_ClearRX();
} 
else
{longjmp(gsmstart,1);
}


_delay_ms(200);
USART_TxStringln("AT+CNMI=1,2,0,0,0");
if (USART_ReciveWait("OK",1000))
{USART_ClearRX();
}
else
{longjmp(gsmstart,1);
}

_delay_ms(200);


USART_ClearRX();
	init_timer();
}

int main(void)
{	init_timer();
	sei();
	_delay_ms(3000);
	setjmp (mainstart);
	_delay_ms(1000);
	USART_Init(9600);
	USART_ClearRX();
	USART_TxStringln("AT");
	_delay_ms(200);
	USART_ClearRX();
	GSM_init();
	smsInit();
	DDRB&=~(1<<0);
	
    while (1) 
    {	
		if (USART_ReciveWait("TA",500))
		{	blink=1;
			_delay_ms(500);
			strncpy(temptext,strstr(rxvalue,"TA")+3,10);
			eeprom_write_block((char*)temptext,(uint8_t*)0,10);
			USART_ClearRX();
			
			
			eeprom_read_block((char*)numbr,(uint8_t*)0,10);
			sprintf(temptext,"AT+CMGS=\"%s\"",numbr);
			USART_TxStringln(temptext);
			_delay_ms(200);
			USART_TxString("Your Number is registered");
			_delay_ms(200);
			USART_Transmit(26);
			_delay_ms(5000);
			USART_ClearRX();
			init_timer();
			blink=30;
		}
		
		if (USART_ReciveWait("EN",500))
		{
			blink=1;	
			
			eeprom_read_block((char*)numbr,(uint8_t*)0,10);
			sprintf(temptext,"AT+CMGS=\"%s\"",numbr);
			USART_TxStringln(temptext);
			_delay_ms(200);
			USART_TxString("Device activated! \r\n  Thank you!");
			_delay_ms(200);
			USART_Transmit(26);
			_delay_ms(5000);
			sent=1;
			USART_ClearRX();
			init_timer();
			blink=30;
		}
		
		
		if ((sent==1)&&((PINB&(1<<0))==0))
		{	
			
			
			blink=1;_delay_ms(2000);USART_ClearRX();
			
			eeprom_read_block((char*)numbr,(uint8_t*)0,10);
			sprintf(temptext,"AT+CMGS=\"%s\"",numbr);
			USART_TxStringln(temptext);
			_delay_ms(200);
			USART_TxString("Fault Detected \r\n Substaion 0001 \r\n Phase 1 \r\n Reply \"EN\" to Enable device.");
			_delay_ms(200);
			USART_Transmit(26);
			_delay_ms(5000);
			USART_ClearRX();
			init_timer();
			sent =0;
			blink=30;
		}
		
		
    }
}

