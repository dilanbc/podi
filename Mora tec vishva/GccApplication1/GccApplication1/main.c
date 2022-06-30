#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

uint16_t Reading=0; //analog Reading variable
uint16_t ReadADC(uint8_t ADCchannel); //analog reading function
void servoangle(uint8_t pin);

int main(void)
{
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));   // prescaler 128
	ADMUX |= (1<<REFS0)|(1<<REFS1);					//internal 2.56 v ref
	ADCSRA |= (1<<ADEN);                            // Turn on ADC
	DDRD|=(1<<4);
	while (1)
	{
		Reading=(ReadADC(0)*0.25024438); //calibrated number
		if (Reading>30)
		{servoangle(1);
		} 
		else
		{servoangle(0);
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
void servoangle(uint8_t pin){

if (pin)
{
	for(uint8_t j=0;j<100;j++){
		PORTD|=(1<<4);
		for(uint8_t i=0;i<10;i++){//1ms delay
			_delay_us(100);
		}
		PORTD&=~(1<<4);
		
		
		
		for(uint8_t i=0;i<10;i++){
			_delay_us(100);
		}
	}
} 
else
{for(uint8_t j=0;j<100;j++){
	
	PORTD|=(1<<4);
	for(uint8_t i=0;i<15;i++){//1.5ms
		_delay_us(100);
	}
	PORTD&=~(1<<4);
	
	for(uint8_t i=0;i<15;i++){
		_delay_us(100);
	}
}
}
	
	
	
	
}