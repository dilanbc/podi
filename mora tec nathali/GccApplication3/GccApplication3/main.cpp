#define F_CPU 8000000
#include <avr/io.h>


int main(void)
{
	DDRD=0b1;
    while (1) 
    {
		if (PINC&(0b1))
		{PORTD=0b1;
		} 
		else
		{PORTD=0b0;
		}
    }
}

