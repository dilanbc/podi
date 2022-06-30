#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

#define stepperDDR DDRC
#define stepperPORT PORTC
#define DIRL 4
#define STEPL 5

void stepperMotor(uint8_t side,uint16_t cycle);

int main(void)
{
    stepperDDR|=(1<<DIRL)|(1<<STEPL);
	DDRC|=(1<<6);
	PORTC|=(1<<6);
	 stepperMotor(1,500);
    while (1) 
    {
    }
}

void stepperMotor(uint8_t side,uint16_t cycle){

stepperPORT=(stepperPORT&0b11100111)|(side<<3);

for(uint16_t i=0;i<cycle;i++){
stepperPORT|=(1<<STEPL);
_delay_us(1000);
stepperPORT&=~(1<<STEPL);
_delay_us(1000);	
}
}