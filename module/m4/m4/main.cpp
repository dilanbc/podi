
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "LCDI2C.h"
#include "USART.h"

uint16_t cycle=0;
char lcdtx[15];

uint16_t ReadADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	return ADCW;
}

int main(void)
{
	USART_Init(9600);
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));   // prescaler 128
	ADMUX |= (1<<REFS0)|(1<<REFS1);                            // Intarnal 2.56 ref
	ADCSRA |= (1<<ADEN);                            // Turn on ADC
	ADCSRA |= (1<<ADSC);                            // Do an initial conversion
	LcdInit();
	LcdSetCursor(0,0,"Please wait . .");
	_delay_ms(3000);
	DDRA|=(1<<1);//relay pin output
	LcdCommand(LCD_CLEARDISPLAY);
	while (1)
	{	uint16_t Readtemp=ReadADC(0)/4.05;
		sprintf(lcdtx,"Tempura. %uC",Readtemp);
		LcdSetCursor(0,0,lcdtx);
		
		if (Readtemp>37)
		{LcdSetCursor(0,1,"High Tempurature");
			PORTA&=~(1<<1);//off relay
			_delay_ms(3000);
			LcdCommand(LCD_CLEARDISPLAY);
			LcdSetCursor(0,0,"You Cannot Enter");
			LcdSetCursor(3,1," The shop");
			_delay_ms(1000);

			USART_TxStringln("There is a person   ");
			USART_TxStringln("trying to enter the");
			USART_TxStringln("shop and he/she has");
			USART_TxStringln("abnormal health condition");
			_delay_ms(1000);
			LcdCommand(LCD_CLEARDISPLAY);
		}
		else
		{LcdSetCursor(0,1,"Openning. .     ");
			PORTA|=(1<<1);//on relay
			_delay_ms(3000);
			
		}
	}
}

