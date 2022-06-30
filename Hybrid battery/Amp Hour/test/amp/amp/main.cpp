/*
 * amp.cpp
 *
 * Created: 9/26/2020 11:32:25 PM
 * Author : Dilan
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "I2C.h"
#include "LCDI2C.h"
#include "USART.h"

#define resistor 1000
double ampH=0;
float readAdc(uint8_t channel);
void adcInit();

int main(void)
{	USART_Init(9600);
   LcdInit();
   adcInit();
   LcdSetCursor(0,4,"welcome");
   DDRB|=(1<<0); //rl as output
   
   DDRB&=~(1<<1);
   PORTB|=(1<<1); //sw pullup
   
   while((PINB&(1<<PINB1))>0){}
    
	_delay_ms(500);
	PORTB|=(1<<0);
	while(1){
		_delay_ms(1000);
		float adcT=readAdc(0);
			if ((adcT<8.5)&&(adcT>6))
			{
			float shortAmp=	((adcT/resistor)*1000/3600);
			ampH=ampH+ shortAmp;
			USART_Txfloat(adcT,5,5);
			USART_TxString(" V     ");
			USART_Txfloat((adcT/resistor)*1000,5,5);
			USART_TxString("mA   ");
			USART_Txfloat(shortAmp,5,5);
			USART_TxString("mAH    ");
			USART_Txfloat(ampH,5,8);
			USART_TxStringln("");
		}
	}
}
void adcInit(){
	ADMUX|=1<<REFS0;  // reference AVCC
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //enable adc prescaler is 128
	
}

float readAdc(uint8_t channel){
	ADMUX=(ADMUX&0xf0)|(0b111&channel);
	ADCSRA|=(1<<ADSC);   // start conversion
	while(!(ADCSRA & (1<<ADIF)));   // wait for ADIF conversion complete return
	ADCSRA|=(1<<ADIF);   // clear ADIF when conversion complete by writing 1
	return ((ADC/102.4)); //return calculated ADC value
	
}
