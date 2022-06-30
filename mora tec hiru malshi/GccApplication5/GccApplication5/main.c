#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

#define stepDDR DDRC
#define stepPORT PORTC
#define DIRL 3
#define DIRR 4
#define STEPL 5
#define STEPR 6

void stepperM(uint8_t side,uint16_t cycle);

int main(void)
{	
	stepDDR|=(1<<DIRL)|(1<<DIRR)|(1<<STEPL)|(1<<STEPR);
	stepperM(3,5000);
	while (1)
	{
		
	}
}

void stepperM(uint8_t side,uint16_t cycle){
	//0 right  1 reverse  2 forward  3left
	PORTC=(PORTC&0b11100111)|(side<<3);


	for(uint16_t i=0;i<cycle;i++){
		stepPORT|=(1<<STEPL)|(1<<STEPR);
		_delay_us(1000);
		stepPORT&=~((1<<STEPL)|(1<<STEPR));
		_delay_us(1000);
	}
}

