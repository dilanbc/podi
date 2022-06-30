/*
 * 1602.cpp
 *
 * Created: 5/13/2020 5:52:38 AM
 * Author : Dilan
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "LCDI2C.h"

void InitADC();
uint16_t ReadADC(uint8_t ADCchannel);

int main()
{
InitADC();
 LcdInit(0x27);
USART_Init(9600);
 LcdSetCursor(0,2,"Dilan Lakshitha  Kandegedara Guana");
 
 /*for(uint8_t i;i<5;i++){
	 LcdCommand(LCD_CURSORSHIFT|LCD_DISPLAYMOVE|LCD_MOVELEFT);
	 _delay_ms(500);
 }*/
 
 while (1)
 {
	 
	 
	USART_TxNumber(ReadADC(1));
	 USART_TxString("\n");
	 
 }

	
}

void InitADC()
{
	// Select Vref=AVcc
	ADMUX |= (1<<REFS0);
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


