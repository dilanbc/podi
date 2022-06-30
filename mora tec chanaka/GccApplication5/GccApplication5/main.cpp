#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

#define stepDDR DDRC
#define stepPORT PORTC
#define DIRL 3
#define STEPL 5

void stepperM(bool side,uint16_t cycle);

int main(void)
{
    stepDDR|=(1<<DIRL)|(1<<STEPL);
	 stepperM(0,5000);
    while (1) 
    {
    }
}

void stepperM(bool side,uint16_t cycle){

if (side)
{stepPORT|=(1<<DIRL);
} 
else
{stepPORT&=~(1<<DIRL);
}


for(uint16_t i=0;i<cycle;i++){
stepPORT|=(1<<STEPL);
_delay_us(1000);
stepPORT&=~(1<<STEPL);
_delay_us(1000);	
}
}

