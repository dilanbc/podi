
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "LCDI2C.h"
volatile bool readan=0;
volatile uint16_t analogRead=0;
volatile uint16_t tempNum;
volatile uint16_t counter=0;
volatile uint16_t lastsave=0;
volatile uint16_t finale=0;


volatile float unit=0;
volatile uint8_t watt=0;

char lcddata[20];
int main(void)
{
   ADMUX|=(1<<REFS1)|(1<<REFS0);
   ADCSRA|=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
   if (readan)
   {ADMUX=(ADMUX&0xF0)|(readan);
   }
   else
   {ADMUX=(ADMUX&0xF0)|(readan);
   }
   ADCSRA|=(1<<ADSC);
   DDRB|=(1<<0);
   PORTB|=(1<<0);
  sei();
   
   LcdInit();LcdInit();LcdInit();LcdInit();
    while (1) 
    {	sprintf(lcddata,"%04u",watt);
		LcdSetCursor(0,0,lcddata);
		
    }
}

ISR(ADC_vect){
	analogRead=ADC;
	
		
		
		tempNum=analogRead;//30
		if (tempNum>lastsave)
		{lastsave=tempNum;
		}
		counter++;
		if (counter>20000)
		{finale=lastsave;
			lastsave=0;
			counter=0;
			watt=finale*2;
			unit+=watt/180000000;
			
			
			
			
		}
	

	ADMUX=(ADMUX&0xF0)|(readan);

	ADCSRA|=(1<<ADSC);
	
}