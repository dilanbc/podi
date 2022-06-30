// USART 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>



void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
void USART_TxString(char *ptr_string);
void USART_TxStringln(char *ptr_string);
void USART_TxNumber(int32_t number,uint8_t dtype);
void USART_Txfloat(float number,uint8_t length,uint8_t deciP);
bool USART_ReciveWait(char*wait,int16_t timeout ,bool deleteWait);
void USART_ClearRX();

volatile uint16_t distance=0;
#define rxshiftNum 4
 volatile uint8_t rxvalue[rxshiftNum];
 volatile uint8_t chari=0;

void USART_Init( unsigned int ubrr)
{	ubrr=F_CPU/16/ubrr-1;
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); 
	sei();
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_TxString(char *ptr_string)
{
	while(*ptr_string)
	USART_Transmit(*ptr_string++);// Loop through the string and transmit char by char
}
void USART_TxStringln(char *ptr_string)
{
	while(*ptr_string){
	USART_Transmit(*ptr_string++);// Loop through the string and transmit char by char
	}
	USART_Transmit(13);
	USART_Transmit(10);
}


void USART_Txfloat(float number,uint8_t length,uint8_t deciP){
	
	char fString[10];
	dtostrf(number,length,deciP,fString);
	USART_TxString(fString);
	
}


ISR(USART_RX_vect){
	volatile uint8_t rxchar=UDR0;
	
	
	
	if (rxchar==0xff)
	{chari=0;
	rxvalue[chari]=	rxchar;
	} 
	else
	{chari++;
	rxvalue[chari]=	rxchar;
	
	
	if (chari==3)
	{
		if (rxvalue[3]!=0)
		{ volatile uint16_t confrm=(rxvalue[0]+rxvalue[1]+rxvalue[2])&0x00FF;
			if (confrm==rxvalue[3])
			{volatile uint16_t discal=(rxvalue[1]<<8)|rxvalue[3];
				distance=discal;
			}
		} 
		else
		{distance=0;
		}
			
		
	}
	}
	
	
	
		
}


