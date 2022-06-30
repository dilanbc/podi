#define F_CPU 8000000
#include <avr/io.h>

uint16_t anx=0;
uint16_t anY=0;

uint16_t ReadADC(uint8_t ADCchannel);

int main(void)
{	
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));   // prescaler 128
	ADMUX |= (1<<REFS0);					//external vcc
	ADCSRA |= (1<<ADEN);                            // Turn on ADC
	
    DDRC=0xFF;
	PORTA|=(1<<0);
    while (1) 
    {anx=ReadADC(7);
		if (anx>512+200)
		{PORTC|=(1<<3);
			PORTC&=~(1<<2);
		}
		else if(anx<512-200){
		PORTC|=(1<<2);
		PORTC&=~(1<<3);
		}
		else{
		PORTC&=~(1<<3);
		PORTC&=~(1<<2);
		}
		
		anY=ReadADC(6);
		if (anY>512+200)
		{PORTC|=(1<<0);
			PORTC&=~(1<<1);
		}
		else if(anY<512-200){
			PORTC|=(1<<1);
			PORTC&=~(1<<0);
		}
		else{
			PORTC&=~(1<<0);
			PORTC&=~(1<<1);
		}
		
		if (PINA&0b1)
		{PORTC&=~(1<<7);
		} 
		else
		{PORTC|=(1<<7);
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
