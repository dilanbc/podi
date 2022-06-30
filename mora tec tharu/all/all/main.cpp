#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t TimerCal=0;// variable for collect echo data
uint16_t ultraINT0=0;
volatile uint8_t rchar;

int main(void)
{
   /*IR Remote*/
   DDRA&=~((1<<0)|(1<<1));//ir as input
   DDRC=0xFF;//led and speaker
   DDRA|=(0b1111<<4);//motor
    
    DDRD|=(1<<3); //D4 as output
	TCCR0|=(1<<WGM01);//Enable Compare match mode
	TCCR0|=(1<<CS11);//Start timer  prescaler =8
	TCNT0=0;
	OCR0=10;
	/*register value= time*(clock speed/prescale)
	register value=0.000001*(8000000/1)
	register value=10*/
	TIMSK|=(1<<OCIE0);//enable timer Compare inturrept
	
	
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
		PORTD&=~(1<<3);//TRIG pin low
		_delay_us(50);//wait 50 micro sec
		PORTD|=(1<<3);//TRIG pin high
		_delay_us(50);//wait 50 micro sec
		PORTD&=~(1<<3);////TRIG pin low
		while(!(PIND&(1<<2)))//wait for pulse
		TimerCal=0;//rest timer
		while((PIND&(1<<2)))////wait for pulse down
		ultraINT0=TimerCal;//copy timer value
		
		
		if (ultraINT0<200)
		{PORTC|=(1<<7);//buzzer start
			PORTA=(PORTA&0x0F)|(0b0000<<4);//stop
		}
		else{
		
		
		if (PINA&(1<<0))
		{PORTA=(PORTA&0x0F)|(0b0110<<4);//right
		PORTC^=(1<<0);
		}
		else if((PINA&(1<<1))){
		PORTA=(PORTA&0x0F)|(0b1001<<4);//left
		PORTC^=(1<<1);
		}
		else{
		PORTC&=~(1<<7);	//buzzer stop
		PORTA=(PORTA&0x0F)|(0b0101<<4);//go	
		PORTC&=~(0b11<<0);
		}
		
		}
		
		
		
		
		_delay_ms(100);
    }
}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCal++;
	TCNT0=0;
	
}

ISR(USART_RXC_vect){//F B L R s
	
	rchar=UDR;
	if (rchar)
	{
		switch(rchar){
		case 70:PORTA=(PORTA&0x0F)|(0b0101<<4);break;//fowrd
		case 66:PORTA=(PORTA&0x0F)|(0b1010<<4);break;//back
		case 76:PORTA=(PORTA&0x0F)|(0b1001<<4);break;//left
		case 82:PORTA=(PORTA&0x0F)|(0b0110<<4);break;//right
		case 83:PORTA=(PORTA&0x0F)|(0b0000<<4);break;//stop
		}
		_delay_ms(100);
		rchar=0;
	}
}

