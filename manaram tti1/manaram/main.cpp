/*
 * manaram.cpp
 *
 * Created: 10/7/2021 11:03:02 PM
 * Author : dilan
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>

#define startDelay 100 //startdelay=time/0.02
volatile bool blinkstate=0;
volatile uint16_t stateRead=0;
volatile uint16_t timerState=0;
volatile bool timerfinish[14]={0};//timer output
volatile bool actfinish[14]={0};
volatile uint8_t timerVal[14]={0};//timer count

void outputcontrol(uint8_t pin, bool state);

int main(void)
{
    DDRA=0b11111111;
	DDRC=0b11111111;
	
	PORTD|=(0b11<<6); //pull up
	
	/*0.5 timer init part*/
	TCCR1B|=(1<<WGM12)|(1<<CS12);
	TCNT1=0;
	OCR1A=15625;
	TIMSK|=(1<<OCIE1A);
	
	/*0.02timer init part*/
	TCCR0|=(1<<WGM01)|(1<<CS02)|(1<<CS00);
	TCNT0=0;
	OCR0=156;
	TIMSK|=(1<<OCIE0);
	
	
	sei();
    while (1) 
    {
		/*stateRead part*/
		stateRead=(~((PIND<<8)|PINB))&0b11111111111111;
	
		
		if (stateRead&0b11111111111111)//input pin
			{timerState=stateRead&0b11111111111111;

			} 
			else//no pin 
			{timerState=0;
				
			}
		////////////////////////////
		if (!(PIND&(1<<6)))//rst
		{
			for(uint8_t i=0;i<14;i++){
				actfinish[i]=timerfinish[i];
			}
		}
		if (!(PIND&(1<<7)))//lamp test
		{
			PORTA=0b11111111;
			PORTC|=0b111111<<2;
		}
		
		/*array eqvel check*/
		uint8_t spkcount=0;
		uint8_t spk=0;
		for(uint8_t i=0;i<14;i++){
		if (!(timerfinish[i]))
		{spkcount++;
		}
		if (timerfinish[i]==actfinish[i])
		{spk++;
		} 
		else
		{break;
		}
		}
		
		if ((spkcount!=14)&&(spk<14))
		{PORTC|=(1<<1);PORTC|=(1<<0);
		} 
		else
		{PORTC&=~(1<<1);PORTC&=~(1<<0);
		}
		/*array eqvel check*/
    }
}
ISR(TIMER1_COMPA_vect){//blink
TCNT1=0;
blinkstate=!blinkstate;
for(uint8_t i=0;i<14;i++){
	if (timerfinish[i])
	{
		if (actfinish[i])
		{outputcontrol(i,1);
		} 
		else
		{outputcontrol(i,blinkstate);
		}
	} 
	else
	{outputcontrol(i,0);
	}
}

}
ISR(TIMER0_COMP_vect){
	TCNT0=0;
	for(uint8_t i=0;i<14;i++){
		if (timerState&(1<<i))//logic active
		{	if (timerVal[i]<startDelay)//timerCounting
		{timerVal[i]++;
		} 
		else
		{timerfinish[i]=1;//finished
		}
			
		}
		else{//logic de active
			
			if (timerfinish[i])//pre acive
			{if (actfinish[i])//check act done
			{timerfinish[i]=0;
			timerVal[i]=0;
			actfinish[i]=0;
			} 
			else
			{
			}
			} 
			else
			{timerfinish[i]=0;
			timerVal[i]=0;
			actfinish[i]=0;
			}
			
			
			
		}
	}
	

}

void outputcontrol(uint8_t pin, bool state)
{
	if (state)
	{
		if (pin<8)
		{PORTA|=(1<<pin);
		} 
		else
		{PORTC|=(1<<(15-pin));
		}
	} 
	else
	{
		if (pin<8)
		{PORTA&=~(1<<pin);
		}
		else
		{PORTC&=~(1<<(15-pin));
		}
	}
}
