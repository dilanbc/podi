 
#define F_CPU 8000000
#include <avr/io.h>


int main(void)
{	
	/*IR Remote*/
	DDRA&=~((1<<0)|(1<<1));//ir as input
   
    while (1) 
    {
		if (PINA&(1<<0))//if PINA0 high
		{
			
		}
		if (PINA&(1<<1))//if PINA1 high
		{
			
		}
    }
}

