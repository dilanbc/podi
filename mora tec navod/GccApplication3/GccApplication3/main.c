#define F_CPU 8000000
#include <avr/io.h>


int main(void)
{
    DDRA=0b11;
    while (1) 
    {
		if (!(PINC&(1<<1)))//if PINC1 off?
		{PORTA|=(1<<0);//bulb on
		} 
		else
		{PORTA&=~(1<<0);//bulb off
		}
		if ((PINC&(1<<0)))//pinc0 on?
		{PORTA|=(1<<1); //PORTA 1 on busser
		}
		else
		{PORTA&=~(1<<1);//PORTA 1 off
		}
		
    }
}

