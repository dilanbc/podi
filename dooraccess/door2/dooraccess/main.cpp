/*
 * dooraccess.cpp
 *
 * Created: 12/27/2020 11:30:14 PM
 * Author : Dilan
 */ 
#define F_CPU 8000000UL


#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "spi.h"
#include "RC522.h"

volatile int st=0;
volatile int r=0;
uchar str[16];
uchar status;
uint8_t eepromR[4][8];


bool reading();
void newCard();


int main(void)
{	
	/*init card*/
	DDRC&=~(1<<0);
	PORTC|=(1<<0);
	_delay_ms(100);
	if (!((PINC&(1<<0))>>0))//programming mode
	{	newCard();
	}
	/*end initcard*/
	

   
   DDRB|=(1<<0);
   PORTB&=~(1<<0);
   
   _delay_ms(1000);
   
   for(uint8_t i=0;i<4;i++){
	   for(uint8_t j=0;j<8;j++){
		   eepromR[i][j]=eeprom_read_byte((uint8_t*)((8*i)+j));
		   
	   }
   }
   
   
 
 
   //MFRC522_HardReset();
   SPI_Init();
   MFRC522_Init();   // Initialize RC522 module


 
   
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
			
		}

		for(uint8_t i=0;i<4;i++){
		for(uint8_t j=0;j<8;j++){
			if (ReadingCard[j]==eepromR[i][j])
			{
				state=1;
			} 
			else
			{
				state=0;
			}
		}
		if (state==1)
		{break;
		}
		
		}
		
	}
		return state;
}
	
	
	


void newCard(){
	DDRB|=(1<<6);
	PORTB|=(1<<6);

	
	SPI_Init();
	MFRC522_Init();   // Initialize RC522 module
	
	/*one*/
	PORTB|=(1<<6);
	while(1){
		status = MFRC522_Request(PICC_REQIDL, str);
		//Anti-collision, return card serial number 4 bytes
		status = MFRC522_Anticoll(str);
		memcpy(serNum, str, 5);

		if (status == MI_OK)   //if ther is a new card detected
		{
			SetFormatRDM630(); // take serNum and format it and retern read
			for(uint8_t i=0;i<8;i++){
				eeprom_write_byte((uint8_t*)(0+i),read[i]);
				
			}
									
			break;
			
		}
		
	}
	_delay_ms(500);
	for(uint8_t i=0;i<25;i++){
		PORTB|=(1<<6);
		_delay_ms(100);
		PORTB&=~(1<<6);
		_delay_ms(100);
	}
	
	/*two*/
	_delay_ms(2000);
	PORTB|=(1<<6);
	while(1){
		status = MFRC522_Request(PICC_REQIDL, str);
		//Anti-collision, return card serial number 4 bytes
		status = MFRC522_Anticoll(str);
		memcpy(serNum, str, 5);

		if (status == MI_OK)   //if ther is a new card detected
		{
			SetFormatRDM630(); // take serNum and format it and retern read
			for(uint8_t i=0;i<8;i++){
				eeprom_write_byte((uint8_t*)(8+i),read[i]);
				
			}
			
			break;
			
		}
		
	}
	
	
	_delay_ms(500);
	for(uint8_t i=0;i<25;i++){
		PORTB|=(1<<6);
		_delay_ms(100);
		PORTB&=~(1<<6);
		_delay_ms(100);
	}
	
	/*three*/
	_delay_ms(2000);
	PORTB|=(1<<6);
	while(1){
		status = MFRC522_Request(PICC_REQIDL, str);
		//Anti-collision, return card serial number 4 bytes
		status = MFRC522_Anticoll(str);
		memcpy(serNum, str, 5);

		if (status == MI_OK)   //if ther is a new card detected
		{
			SetFormatRDM630(); // take serNum and format it and retern read
			for(uint8_t i=0;i<8;i++){
				eeprom_write_byte((uint8_t*)(16+i),read[i]);
				
			}
			
			break;
			
		}
		
	}
	
	_delay_ms(500);
	for(uint8_t i=0;i<25;i++){
		PORTB|=(1<<6);
		_delay_ms(100);
		PORTB&=~(1<<6);
		_delay_ms(100);
	}
	
	/*four*/
	_delay_ms(2000);
	PORTB|=(1<<6);
	while(1){
		status = MFRC522_Request(PICC_REQIDL, str);
		//Anti-collision, return card serial number 4 bytes
		status = MFRC522_Anticoll(str);
		memcpy(serNum, str, 5);

		if (status == MI_OK)   //if ther is a new card detected
		{
			SetFormatRDM630(); // take serNum and format it and retern read
			for(uint8_t i=0;i<8;i++){
				eeprom_write_byte((uint8_t*)(24+i),read[i]);
				
			}
			
			break;
			
		}
		
	}
	_delay_ms(500);
	for(uint8_t i=0;i<25;i++){
		PORTB|=(1<<6);
		_delay_ms(100);
		PORTB&=~(1<<6);
		_delay_ms(100);
	}
	PORTB&=~(1<<6);
	while(1);
}