#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t rchar;

int main(void)
{	
	DDRC=0xFF;
	
	/*Set baud rate */
	UBRRH = (unsigned char)(51>>8);
	UBRRL = (unsigned char)51;
	/*Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<RXCIE);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
	sei();
    
    while (1) 
    {
		if (rchar==107)
		{PORTC|=(1<<0);
		rchar=0;	
		}
		
		if (rchar==108)
		{PORTC&=~(1<<0);
			rchar=0;
		}
    }
}

ISR(USART_RXC_vect){//A,B,C,D ???
	
	rchar=UDR;

	
}

