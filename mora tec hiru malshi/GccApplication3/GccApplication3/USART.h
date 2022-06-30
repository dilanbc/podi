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
uint8_t USART_ReciveWait(char*wait,int16_t timeout ,uint8_t deleteWait);
void USART_ClearRX();
void USART_getLocation();
float longitude=0;
float laditute=0;

char num[]="";
char rchar=0;
#define rxshiftNum 20
#define lon 7
#define lad 80
char rxvalue[rxshiftNum];

void USART_Init( unsigned int ubrr)
{	ubrr=F_CPU/16/ubrr-1;
	/*Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
	sei();
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	
	/* Put data into buffer, sends the data */
	UDR = data;
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



ISR(USART_RXC_vect){//A,B,C,D ???
	
	char rchar=UDR;
	
	uint8_t i=0;
	
	while(rxvalue[i]){
		i++;
		if (i==rxshiftNum)
		{break;
		}
	}
	if (i<rxshiftNum)
	{rxvalue[i]=rchar;
	}
	else
	{
		for(uint8_t i=0;i<rxshiftNum-1;i++){
			rxvalue[i]=rxvalue[i+1];
		}
		rxvalue[rxshiftNum-1]=rchar;
		
	}
	
	
}




uint8_t USART_ReciveWait(char*wait,int16_t timeout,uint8_t deleteWait){
	uint8_t waitbool=0;
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
	
	return waitbool;
}

void USART_ClearRX(){
	for(uint8_t i=0;i<rxshiftNum;i++){
		rxvalue[i]=NULL;
	}
	
}

char filtere[9];
void USART_getLocation(){
	USART_TxStringln("ATE1"); //Enable Echo if not enabled by default
	_delay_ms(1000);

	USART_TxStringln("AT+CGATT=1"); //Set the SIM800 in GPRS mode
	_delay_ms(1000);

	USART_TxStringln("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\" "); //Activate Bearer profile
	_delay_ms(1000);

	USART_TxStringln("AT+SAPBR=3,1,\"APN\",\"RCMNET\" "); //Set VPN options => 'RCMNET' 'www'
	_delay_ms(2000);

	USART_TxStringln("AT+SAPBR=1,1"); //Open bearer Profile
	_delay_ms(2000);

	USART_TxStringln("AT+SAPBR=2,1"); //Get the IP address of the bearer profile
	_delay_ms(1000);
	
	
	 USART_TxStringln("AT+CIPGSMLOC=1,1"); _delay_ms(5000); //Request for location data
	 
	  //Sample Output for AT+CIPGSMLOC=1,1   ==> +CIPGSMLOC: 0,75.802460,26.848892,2019/04/23,08:32:35 //where 26.8488832 is Lattitude and 75.802460 is longitute
	  strncpy(filtere,strstr(rxvalue,",")+1,8);; //Find the position of 1st comma
	   longitude=atoi(filtere);
	  
	  strncpy(filtere,strstr(rxvalue,",")+1,8);; //Find the position of 2nd comma
	  laditute=atoi(filtere);
}
