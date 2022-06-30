#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>



void servoangle(uint8_t pin);

int main(void)
{
	
	DDRD|=(1<<4);
	DDRC|=(1<<2);
	while (1)
	{	PORTC|=(1<<2);
		servoangle(0);
		_delay_ms(100);
		PORTC&=~(1<<2);
		servoangle(1);
		_delay_ms(100);
	}
}


void servoangle(uint8_t pin){

	if (pin)
	{
		for(uint8_t j=0;j<100;j++){
			PORTD|=(1<<4);
			for(uint8_t i=0;i<10;i++){
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
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
		PORTD&=~(1<<4);
		
		
		
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
	}
}




}