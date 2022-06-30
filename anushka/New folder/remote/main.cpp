#define F_CPU 8000000  //occilator speed 8Mhz
#include <avr/io.h> 
#include <avr/interrupt.h>  //interrupt library
#include <util/delay.h>  //delay library

volatile char letter=0;  //for  saving incoming character

int main(void)
{	
	DDRF|=(1<<2)|(1<<1)|(1<<0);  // define led as output (PF0,PF1,PF2)
	UCSR0B|=(1<<RXCIE0)|(1<<RXEN0)|(1<<RXB80);//setup rx port
	/*RXCIE0=RX Complete Interrupt Enable 0*/
	/*RXEN0=Receiver Enable 0*/
	/*RXB80=Receive Data Bit 8 */
	
	UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);// character size 8 bit
	UBRR0=207;// from table oscillator speed 8Mhz baud rate 2400 -207
	
	DDRE|=(1<<3);// define  output (PE3) lamp
	
	TCCR3A|=(1<<COM3A1)|(1<<WGM30);
	/*COM3A1= setup pwm (from fast pwm table )*/
	/*WGM30= 8bit pwm*/
	
	TCCR3B|=(1<<CS30);//prescaler =1
	TCCR3C|=(1<<FOC3A);//force pwm
	OCR3A=0;//pwm size(brighness control)
	sei();//enable globle inturrept
	
	
    while (1) 
    {
	if (letter)// check whether a letter received??
	{	uint8_t pwm=50;//pwm range control
		if(letter=='A'){//
		PORTF=0b001;//indicator for on/off - 'A' led
			if (OCR3A)//check bulb status?
			{OCR3A=0; //off bulb
			} 
			else
			{OCR3A=255;// on bulb
			}
		
		}
		
		else if(letter=='B'){
			PORTF=0b010;//indicator for increase brightness - 'B' led
			if (OCR3A<=(255-pwm))//check increasing ability
			{OCR3A+=pwm;//increasing brightness 
			}
		}
		
		else if(letter=='C'){
			PORTF=0b100;//indicator for decrease  brightness - 'C' led
			if (OCR3A>=pwm)//check decreasing ability
			{OCR3A-=pwm;//decreasing brightness 
			}
		}
		_delay_ms(50);	//delay for indicate
		letter=0;//clearing previous letter 
		UCSR0B|=(1<<RXEN0);//enable character reading
	}
	
	else{
	PORTF=0b000;	
	}
		_delay_ms(100);
    }
}

ISR(USART0_RX_vect){
	
	letter=UDR0;// assign letter which received
	UCSR0B&=~(1<<RXEN0);//disable character reading
}

