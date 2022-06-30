/*
 * rfid test.cpp
 *
 * Created: 7/29/2021 2:28:30 PM
 * Author : dilan
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "spi.h"
#include "RC522.h"

uchar str[16];
uchar status;
uint8_t ReadingCard [8]={};
void reading();

int main(void)
{
    SPI_Init();
    MFRC522_Init();   // Initialize RC522 module
	
    while (1) 
    { reading();
		_delay_ms(100);
    }
}

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
			ReadingCard[i]=read[i];
			
		}
		
		
		
	}
	
	
	
}