#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD.h"
#include "I2C.h"

 
#define AMS5915_DEFAULT_ADDRESS  0x28
uint8_t pressureH = 0; // The second byte of data
uint8_t pressureL = 0; // The first byte of data
uint16_t pressure = 0; // Variable holding the temperature

char  lcddata[20];

int main(void)
{
	LcdInit();
	TWIInit();
	
	
	_delay_ms(1000);
	
	
	while (1)
	{
		TWIStart();//start twi
		TWIWriteAddrs(AMS5915_DEFAULT_ADDRESS,0);//device address
		TWIWriteData(1);//register num  pressure
		TWIStart();
		TWIWriteAddrs(AMS5915_DEFAULT_ADDRESS,1);
		pressureH=TWIReadACK();//read high bit
		pressureL=TWIReadNACK();//read low bit
		TWIStop();//end transmission
		
		pressure=0;
		pressure=(pressureH<<8)|(pressureL);
		
		sprintf(lcddata,"pressure-%uC",pressure);
		LcdSetCursor(0,0,lcddata);
		_delay_ms(200);
		
		
		
	}
	
	
	
	
}

