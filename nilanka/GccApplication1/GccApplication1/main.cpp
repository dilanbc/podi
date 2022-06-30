#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"

char lengthcal[4];
char tempcal[4];
bool val[4];

#define DHT11_PIN 7
uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;
void Request();
void Response();
uint8_t Receive_data();

int main(void)
{
   USART_Init(2400);
   DDRA=0xFF; 
 
    while (1) 
    {	
		if (rxvalue[0]>0)
		{
			strncpy(lengthcal,strstr(rxvalue,"ST")+2,4);
			strncpy(tempcal,strstr(rxvalue,"ST")+7,2);
			
			int tempu=atoi(tempcal);
			for(uint8_t i=0;i<2;i++){
				val[i]=(lengthcal[i]-48);
				if (val[i])
				{PORTA|=(1<<(3+i));
				}
				else
				{PORTA&=~(1<<(3+i));
				}
			}
			
			for(uint8_t i=2;i<4;i++){
				val[i]=(lengthcal[i]-48);
				if (val[i]&&((PIND&(1<<(3+i)))))
				{PORTA|=(1<<(3+i));
				}
				else
				{PORTA&=~(1<<(3+i));
				}
			}
			
			
		
	
		

	
			if (I_Temp>tempu)
			{
			PORTA|=(1<<7);//fan
			}
			else{
			PORTA&=~(1<<7);
			}
		
			
			
			
		}
		
		UCSRB&=~((1<<RXEN)|(1<<RXCIE));
		Request();		/* send start pulse */
		Response();		/* receive response */
		I_RH=Receive_data();	/* store first eight bit in I_RH */
		D_RH=Receive_data();	/* store next eight bit in D_RH */
		I_Temp=Receive_data();	/* store next eight bit in I_Temp */
		D_Temp=Receive_data();	/* store next eight bit in D_Temp */
		CheckSum=Receive_data();/* store next eight bit in CheckSum */
		UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	
		char send[2];
		send[0]=((PIND&(1<<5))>>5)+48;
		send[1]=((PIND&(1<<6))>>6)+48;
		USART_TxString(send);
		_delay_ms(1000);
    }
}

void Request()						/* Microcontroller send start pulse or request */
{
	DDRD |= (1<<DHT11_PIN);
	PORTD &= ~(1<<DHT11_PIN);		/* set to low pin */
	_delay_ms(20);					/* wait for 20ms */
	PORTD |= (1<<DHT11_PIN);		/* set to high pin */
}

void Response()						/* receive response from DHT11 */
{
	DDRD &= ~(1<<DHT11_PIN);
	while(PIND & (1<<DHT11_PIN));
	while((PIND & (1<<DHT11_PIN))==0);
	while(PIND & (1<<DHT11_PIN));
}

uint8_t Receive_data()							/* receive data */
{
	for (int q=0; q<8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0);	/* check received bit 0 or 1 */
		_delay_us(30);
		if(PIND & (1<<DHT11_PIN))				/* if high pulse is greater than 30ms */
		c = (c<<1)|(0x01);						/* then its logic HIGH */
		else									/* otherwise its logic LOW */
		c = (c<<1);
		while(PIND & (1<<DHT11_PIN));
	}
	return c;
}