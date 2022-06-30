/*
 * ir sens.cpp
 *
 * Created: 8/9/2021 1:14:53 PM
 * Author : dilan
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "setjmp.h" 
#include "USART.h"

jmp_buf mainstart;
char lengthcal[40];//string memory
int gsm;//gsm signal calculation
char numbr[15]={};
bool state=0;
bool sendState=0;

void mainfunctionInit();
bool checkGSM(); //network connect check
bool setupGSM();
void eepromNumR();

int main(void)
{	DDRD|=(1<<2);
	PORTD&=~(1<<2);
   setjmp (mainstart);//reset point
   mainfunctionInit();
   sei();
   
    while (1) 
    {
		
		if (USART_ReciveWait("RG",200))
		{	_delay_ms(100);
			char filtere[8];
			strncpy(filtere,strstr(rxvalue,"RG")+3,10);
			eeprom_write_block((char*)filtere,(uint8_t*)8,10);
			eeprom_read_block((char*)numbr,(uint8_t*)9,9);
			sprintf(lengthcal,"AT+CMGS=\"+94%s\"",numbr);
			USART_TxStringln(lengthcal);
			_delay_ms(200);
			USART_TxString("Your Number is registered");
			_delay_ms(200);
			USART_Transmit(26);
			_delay_ms(200);
			USART_ClearRX();
			_delay_ms(3000);
		}
		
		if (USART_ReciveWait("SMS ON",200))
		{	_delay_ms(100);
			state=1;sendState=0;
			eeprom_write_byte((uint8_t*)0,1);
			eeprom_read_block((char*)numbr,(uint8_t*)9,9);
			sprintf(lengthcal,"AT+CMGS=\"+94%s\"",numbr);
			USART_TxStringln(lengthcal);
			_delay_ms(200);
			USART_TxString("SMS Alert activated");
			_delay_ms(200);
			USART_Transmit(26);
			_delay_ms(200);
			USART_ClearRX();
			_delay_ms(3000);
		}
		
		if (USART_ReciveWait("SMS OFF",200))
		{	_delay_ms(100);
			state=0;
			
			eeprom_write_byte((uint8_t*)0,0);
			eeprom_read_block((char*)numbr,(uint8_t*)9,9);
			sprintf(lengthcal,"AT+CMGS=\"+94%s\"",numbr);
			USART_TxStringln(lengthcal);
			_delay_ms(200);
			USART_TxString("SMS Alert deactivated");
			_delay_ms(200);
			USART_Transmit(26);
			_delay_ms(200);
			USART_ClearRX();
			_delay_ms(3000);
		}
		
	if (((PINB&0b1000000)>>1)&&(!sendState))
    {//PORTD|=(1<<7);
		PORTB|=(1<<0);
		sendState=1;
		if (state)
		{eeprom_read_block((char*)numbr,(uint8_t*)9,9);
			sprintf(lengthcal,"AT+CMGS=\"+94%s\"",numbr);
			USART_TxStringln(lengthcal);
			_delay_ms(200);
			USART_TxString("Motion Detected");
			_delay_ms(200);
			USART_Transmit(26);
			_delay_ms(200);
			USART_ClearRX();
			_delay_ms(5000);
		}
		
    }
	if(!((PINB&0b1000000)>>1)){
		PORTD&=~(1<<7);
		PORTB&=~(1<<0);
		sendState=0;
	}
    }
}

void mainfunctionInit(){
	DDRD|=(1<<7);
	PORTD|=(1<<7);
	DDRB|=(1<<0);
	_delay_ms(40);
	PORTD&=~(1<<7);
	
	if (eeprom_read_byte((uint8_t*)0)==1)
	{state=1;
	} 
	else
	{state=0;
	}
	USART_Init(9600);
	if (!checkGSM())
	{longjmp(mainstart,1);
	}
	setupGSM();
	USART_ClearRX();
	PORTD|=(1<<7);
	_delay_ms(100);
	PORTD&=~(1<<7);
	_delay_ms(100);
	PORTD|=(1<<7);
	_delay_ms(100);
	PORTD&=~(1<<7);
	_delay_ms(100);
}


bool checkGSM(){
	DDRD|=(1<<2);
	PORTD|=(1<<2);
	for(uint8_t i=0;i<20;i++){
		USART_TxStringln("AT");
		if (USART_ReciveWait("OK",1000))
		{
			USART_ClearRX();
			break;
		}
		if (i==19)
		{return 0;
		}
	}
	USART_TxStringln("AT+CPIN?");//sim check
	if (USART_ReciveWait("READY",10000)==1)                 //Command and time out
	{
		USART_ClearRX();
	//	return 1;
	}
	else
	{
		USART_ClearRX();
		return 0;
	}
	
	for(uint8_t i=0;i<20;i++){
	USART_TxStringln("AT+CSQ");  //signal strength
		_delay_ms(500);
		strncpy(lengthcal,strstr(rxvalue,"CSQ: ")+5,2);
		gsm=atoi(lengthcal);
		if (gsm>15)
		{break;
		}
		else{
		USART_ClearRX();
		_delay_ms(1000);		
		}
	USART_ClearRX();	
	}
	
	
	
	
	
	
	
	
}

bool setupGSM(){
	//******SMS Functions
	USART_TxStringln("AT+CMGF=1");
	USART_ReciveWait("OK",1000,1);
	_delay_ms(200);
	USART_ClearRX();
	
	USART_TxStringln("AT+CNMI=1,2,0,0,0");//sms read
	USART_ReciveWait("OK",1000,1);
	_delay_ms(200);
		
	//****Data function
	
	
}

void eepromNumR(){
	
	
	
}
