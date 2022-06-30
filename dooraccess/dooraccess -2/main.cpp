/*
 * dooraccess.cpp
 *
 * Created: 12/27/2020 11:30:14 PM
 * Author : Dilan
 */ 
#define F_CPU 8000000UL



#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "USART.h"
#include "spi.h"
#include "RC522.h"

volatile int st=0;
volatile int r=0;
uchar str[16];
uchar status;
uint8_t eepromR[8];



bool reading(){
	bool state=0;
	uint8_t ReadingCard [8]={};
	
	//Find cards, return card type - Don't remove this sub
	status = MFRC522_Request(PICC_REQIDL, str);
	//Anti-collision, return card serial number 4 bytes
	status = MFRC522_Anticoll(str);
	memcpy(serNum, str, 5);

	if (status == MI_OK)   //if ther is a new card detected
	{
		SetFormatRDM630(); // take serNum and format it and retern read
		for(uint8_t i=0;i<8;i++){
			ReadingCard[i]=read[i];
			USART_TxNumber(ReadingCard[i],16);
			USART_TxString(" ");
		}
		USART_TxStringln("");
		
	for(uint8_t i=0;i<8;i++){
		if (ReadingCard[i]==eepromR[i])
		{state=1;
		}
		else
		{state=0;
		break;
		}
	}	
	}
	
	return state;
	
}


int main(void)
{	
	/*init card*/
/*
	DDRD&=~(1<<1);
	PORTD|=(1<<1);
	_delay_ms(100);
	if (((PIND&(1<<1))==0))//programming mode
	{	DDRB|=(1<<2);
		PORTB|=(1<<2);
		_delay_ms(250);
		PORTB&=~(1<<2);
		_delay_ms(250);
		PORTB|=(1<<2);
		_delay_ms(250);
		PORTB&=~(1<<2);
		_delay_ms(250);
		PORTB|=(1<<2);
		_delay_ms(250);
		PORTB&=~(1<<2);
		_delay_ms(250);
		SPI_Init();
	MFRC522_Init();   // Initialize RC522 module
	while(1){
		status = MFRC522_Request(PICC_REQIDL, str);
		//Anti-collision, return card serial number 4 bytes
		status = MFRC522_Anticoll(str);
		memcpy(serNum, str, 5);

		if (status == MI_OK)   //if ther is a new card detected
		{
			SetFormatRDM630(); // take serNum and format it and retern read
			for(uint8_t i=0;i<8;i++){
				eeprom_write_byte((uint8_t*)i,read[i]);
				
			}
			
			PORTB|=(1<<2);
			_delay_ms(250);
			PORTB&=~(1<<2);
			_delay_ms(250);
			PORTB|=(1<<2);
			_delay_ms(250);
			PORTB&=~(1<<2);
			_delay_ms(250);
			PORTB|=(1<<2);
			_delay_ms(250);
			PORTB&=~(1<<2);
			_delay_ms(250);
			
			break;
		
		}
		_delay_ms(500);
	}
	while(1);
	}*/
	/*end initcard*/
	
	
   USART_Init(9600);
   USART_TxStringln("Started");
   
   DDRB|=(1<<0);
   PORTB&=~(1<<0);
   
   _delay_ms(1000);
   for(uint8_t i=0;i<8;i++){
	   eepromR[i]=eeprom_read_byte((uint8_t*)i);
	   USART_TxNumber(eepromR[i],16);
	   USART_TxString(" ");
   }
   USART_TxStringln("");
   //MFRC522_HardReset();
   SPI_Init();
   MFRC522_Init();   // Initialize RC522 module


  /* 
   for (uint8_t i=0;i<5;i++)
   {USART_TxNumber(Read_MFRC522(i),2);
	   USART_TxStringln("");
   }
   */
   
    while (1) 
    {
				
		if (reading())
		{
			PORTB|=(1<<0);
		} 
		else
		{	
			if (reading())
			{PORTB|=(1<<0);
			} 
			else
			{PORTB&=~(1<<0);
			}
			
		}
		_delay_ms(500);
	}
}

