#define F_CPU 8000000
#include <avr/io.h>


int main(void)
{
   DDRD|=(1<<3);
    while (1) 
    {
		if (PIND&(1<<2))
		{PORTD|=(1<<3);
		} 
		else
		{PORTD&=~(1<<3);
		}
    }
}

