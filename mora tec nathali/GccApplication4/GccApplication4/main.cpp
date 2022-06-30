#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

void stepper(uint16_t rev,bool dir){
	for(uint16_t j=0;j<rev;j++){
		//4,4+5,5,5+6,6,6+7,7,7+4
		if (dir)
		{for(uint8_t i=4;i<8;i++){
			PORTD=(PORTD&0x0F)|(1<<i);
			_delay_ms(100);
			if (i==7)
			{PORTD|=(1<<4);
			}
			else
			{PORTD|=(1<<i+1);
			}
			_delay_ms(100);
		}
	}
	else
	{for(uint8_t i=7;i>3;i--){
		PORTD=(PORTD&0x0F)|(1<<i);
		_delay_ms(100);
		if (i==7)
		{PORTD|=(1<<7);
		}
		else
		{PORTD|=(1<<i-1);
		}
		_delay_ms(100);
	}
}



}

}

void servoangle(uint8_t pin){
	
	for(uint8_t j=0;j<100;j++){
		PORTB|=(1<<pin);
		for(uint8_t i=0;i<10;i++){
			_delay_us(100);
		}
		PORTB&=~(1<<pin);
		
		uint8_t ser=200-10;
		
		for(uint8_t i=0;i<ser;i++){
			_delay_us(100);
		}
	}
	_delay_ms(500);
	for(uint8_t j=0;j<100;j++){
		
		PORTB|=(1<<pin);
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
		PORTB&=~(1<<pin);
		
		uint8_t ser=200-15;
		
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
	}
	
	
	
	
}

int main(void)
{
    DDRD|=0xF0;
	DDRB=0xFF;
	servoangle(0);
	stepper(5,1);
    while (1) 
    {
    }
}

