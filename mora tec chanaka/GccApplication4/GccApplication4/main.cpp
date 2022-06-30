#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "LCD.h"

const uint8_t kaypad[4][4]={{7,8,9,10},{4,5,6,11},{1,2,3,12},{13,0,14,15}};
uint8_t getkey();
char lcddata[20];
uint8_t gl;
uint8_t prevkey=20;
uint16_t Reading;

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
    LcdInit();
	
	ADMUX |= (1<<REFS0)|(1<<REFS1);					//internal 2.56 v ref
	
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));   // prescaler 128
	
	ADCSRA |= (1<<ADEN);//enble adc
		
	
    while (1) 
    {
		
		
		
		
		gl=getkey();
		
		if (gl!=20)
		{prevkey=gl;
		}
		
		Reading=ReadADC(0)/10.23;
		
		if (prevkey==20)
		{sprintf(lcddata,"    %u",Reading);
		} 
		else
		{sprintf(lcddata,"%02u  %u",prevkey,Reading);
		}
		
		
		LcdSetCursor(0,0,lcddata);
		
		
    }
}

uint8_t getkey(){
	DDRC=0b00001111;
	PORTC=0b11110000;//pull up pins
	uint8_t getx;//create variable
	uint8_t gety;
	uint16_t i=0;
	while(1){
	
		
	uint8_t pin=(~(PINC|0x0F));	//check any button is press
	if (pin)
	{	
		
	
		switch(pin){//which button is press
		
		
		case (1<<4) :getx=0; break;
		case (1<<5) :getx=1;break;
		case (1<<6) :getx=2;break;
		case (1<<7) :getx=3;break;
		//DDRC=0b00001111;
		
		} //switch
		DDRC=0b0;
		PORTC=0b0;
		
		DDRC=0b11110000;
		PORTC=0b00001111;
		
		pin=~(PINC|0xF0);
		/*DDRD=0xFF;
		PORTD=pin;	*/
		
		
		switch(pin){
			case (1<<0) :gety=0;break;
			case (1<<1) :gety=1;break;
			case (1<<2) :gety=2;break;
			case (1<<3) :gety=3;break;
			
		} //switch
		
		DDRC=0b00001111;
		PORTC=0b11110000;
		
		
		return kaypad[gety][getx];
		
	}//if
	
	i++;
	if (i>50000)//every 500000cycle it run adc funtion
	{return 20;
	}
	
	}//while 1
}

