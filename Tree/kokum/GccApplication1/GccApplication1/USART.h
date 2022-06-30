// USART 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>


void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
void USART_TxString(char *ptr_string);
void USART_TxStringln(char *ptr_string);
void USART_TxNumber(int32_t number,uint8_t dtype);
bool USART_ReciveWait(char*wait,int16_t timeout ,bool deleteWait);
void USART_clear();
char num[]="";
char rxvalue[511];
uint16_t rxvaluenum=0;

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

void USART_TxNumber(int32_t number,uint8_t dtype=10){
	
	itoa(number,num,dtype);
	USART_TxString(num);
	
}



ISR(USART_RX_vect){
	char rchar=UDR0;
	if (rxvaluenum>510)
	{rxvaluenum=0;
	}
	rxvalue [rxvaluenum]=rchar;
	rxvaluenum++;
	
}



bool USART_ReciveWait(char*wait=0,int16_t timeout=0,bool deleteWait=0){
	bool waitbool=0;
	timeout/=100;
	uint8_t temptimeOut=0;
	if (wait!=0)
	{  _delay_ms(100);
		char*tempwait;
		while(tempwait==0){
			tempwait =strstr(rxvalue,wait);
			//SoftUSART_tx_strln(".");
			//SoftUSART_tx_strln(rxvalue);
			
			_delay_ms(100);
			temptimeOut++;
			if ((temptimeOut>timeout)&(timeout!=0))
			{waitbool=0;
				return waitbool;
				
			}
			
		}
		waitbool=1;
		
	}
	
	if (deleteWait!=1)
	{USART_clear();
	}
	
	return waitbool;
}

void USART_clear(){
	
	uint16_t i;
	while(rxvalue[i]!=0){
		rxvalue[i]=0;
	}
	rxvaluenum=0;
}