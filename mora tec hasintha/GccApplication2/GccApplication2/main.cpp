#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include "LCD.h"

char lcddata[20];
uint16_t Reading=0; //analog Reading variable
uint16_t ReadADC(uint8_t ADCchannel); //analog reading function

int main(void)
{
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));   // prescaler 128
	ADMUX |= (1<<REFS0)|(1<<REFS1);					//internal 2.56 v ref
	ADCSRA |= (1<<ADEN);                            // Turn on ADC
   LcdInit();
   DDRC|=(1<<7);
   DDRC|=(1<<6);
    while (1) 
    {
		Reading=(ReadADC(0)*0.25024438); //calibrated number
		sprintf(lcddata,"temp %03uC",Reading);
		LcdSetCursor(0,0,lcddata);
		
		 if (Reading>30)
		 {PORTC|=(1<<7);
			PORTC&=~(1<<6); 
		 } 
		 else
		 {PORTC&=~(1<<7);PORTC|=(1<<6);
		 }
    }
}

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

