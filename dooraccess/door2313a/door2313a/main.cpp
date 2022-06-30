/*
 * door2313a.cpp
 *
 * Created: 12/30/2020 11:48:28 AM
 * Author : Dilan
 */ 
#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "SoftUART.h"
#include "spi.h"
#include "RC522.h"

volatile int st=0;
volatile int r=0;
uchar str[16];
uchar status;


void reading(){
	
	//Find cards, return card type - Don't remove this sub
	status = MFRC522_Request(PICC_REQIDL, str);
	//Anti-collision, return card serial number 4 bytes
	status = MFRC522_Anticoll(str);
	memcpy(serNum, str, 5);

	if (status == MI_OK)   //if ther is a new card detected
	{
		SetFormatRDM630(); // take serNum and format it and retern read
		for(uint8_t i=0;i<8;i++){
			SoftUSART_TxNumber(read[i]);
			SoftUSART_tx_str(" ");
		}
		SoftUSART_tx_strln("");
		
	}
	
	/*else
	{
		for(uint8_t i=0;i<8;i++){
			USART_TxNumber(read[i]);
		}
		USART_TxStringln("");
		MFRC522_Halt();                       // Command card into hibernation
		_delay_ms(50);                       // wait for low consuption
	}*/
}

int main(void)
{	
	
	SoftUART_Init();
	SoftUSART_tx_strln("Started");
	DDRD|=(1<<6);
	PORTD|=(1<<6);
	_delay_ms(1000);
	
   //MFRC522_HardReset();
   SPI_Init(2400);
   MFRC522_Init();   // Initialize RC522 module


   
   for (uint8_t i=0;i<5;i++)
   {SoftUSART_TxNumber(Read_MFRC522(i),2);
	   SoftUSART_tx_strln("");
   }
   
   
    while (1) 
    {
		_delay_ms(500);
		
		reading();
		SoftUSART_tx_strln("loop");
		
	}
}

