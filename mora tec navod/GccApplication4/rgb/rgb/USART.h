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
uint8_t USART_ReciveWait(char*wait,int16_t timeout ,uint8_t deleteWait);
void USART_ClearRX();

char num[]="";
#define rxshiftNum 10
char rxvalue[rxshiftNum];

void USART_Init( unsigned int ubrr)
{	ubrr=F_CPU/16/ubrr-1;
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
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

void USART_TxNumber(int32_t number,uint8_t dtype){
	
	itoa(number,num,dtype);
	USART_TxString(num);
	
}
void USART_Txfloat(float number,uint8_t length,uint8_t deciP){
	
	char fString[10];
	dtostrf(number,length,deciP,fString);
	USART_TxString(fString);
	
}




