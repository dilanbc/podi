/*
 * motorcon.cpp
 *
 * Created: 1/31/2022 7:46:47 PM
 * Author : dilan
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "USART.h"
#include "LCDI2C.h"
void InitADC();

volatile uint16_t tempNum;
volatile uint8_t counter=0;
volatile uint16_t lastsave=0;
volatile uint16_t finale=0;
char txdatata[20];
char filtere[10]={0};
uint16_t tankRead=0;
uint16_t tankReadf=3000;

#define upperL 5000
#define lowerL 2000

int main(void)
{	USART_Init(2400);
	DDRD|=(1<<2);
	PORTD&=~(1<<2);
    InitADC();
	//SoftUART_Init();
	//SoftUSART_tx_strln("started");
	DDRB|=(1<<0);// motor as output
	PORTB&=~(1<<0);//motor off
	//PORTB|=(1<<0);
	PORTB|=(1<<5);// pin pull up
	LcdInit();
	LcdSetCursor(1,0,"FYP Water Pump");
	_delay_ms(2000);
	sei();
	
	
	USART_ClearRX();
	USART_TxStringln("AT");
	_delay_ms(200);
	
	//start setting	
	/*USART_ClearRX();
	USART_TxStringln("AT+C040");
	USART_ReciveWait("OK",500);
	_delay_ms(100);*/
//1st C050,2nd C060, 3rd C040

	
	/*USART_ClearRX();
	USART_TxStringln("AT+B2400");
	USART_ReciveWait("OK",500);
	_delay_ms(100);*/
	
	
	
	/*end setting*/
	USART_ClearRX();
	USART_TxStringln("AT");
	_delay_ms(200);
	
	PORTD|=(1<<2);
    while (1) 
    {	
		float cal=(finale*1000)/220;
		sprintf(txdatata,"C %.2fmA  %02uW       ",cal,finale);
		LcdSetCursor(0,0,txdatata);
		
		if (!(PINB&(1<<5))&&(tankReadf<upperL))//switch on 0?
		{PORTB|=(1<<0);//motor on
			_delay_ms(10000);
			
		}
				
		
		if ((tankReadf>upperL)||(finale<27))//upper limit or ampere
		{PORTB&=~(1<<0);
		}
		
		
		
		
		USART_ClearRX();
		if (USART_ReciveWait("ST",200)){
			USART_ReciveWait("E",200);
			_delay_ms(50);
			strncpy(filtere,strstr(rxvalue,"ST")+2,5);
			tankRead=atoi(filtere);
			if (tankRead>0)
			{tankReadf=tankRead;
			}
			USART_ClearRX();
			
		}
		sprintf(txdatata,"Reading. %05u",tankReadf);
		LcdSetCursor(0,1,txdatata);
		
		
		//sprintf(txdatata,"%u",finale);
		//SoftUSART_tx_strln(txdatata);
		
    }
}

void InitADC()  //meter reading initialized
{
	// Select Vref=AVcc
	ADMUX |= (1<<REFS0)|(1<<REFS1);
	//set prescaller to 128 and enable ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	ADCSRA|=(1<<ADIE);// intruppet
	ADMUX = (ADMUX & 0xF0) | (1 & 0x0F);
	ADCSRA |= (1<<ADSC);
}

ISR(ADC_vect){
	
	tempNum=ADC;//30
	if (tempNum>lastsave)
	{lastsave=tempNum;
	}
	counter++;
	if (counter>200)
	{finale=lastsave;
		lastsave=0;
		counter=0;
		
	}
		
	

	ADCSRA |= (1<<ADSC);
	
	
	
}