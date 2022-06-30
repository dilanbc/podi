/*
 * tx.cpp
 *
 * Created: 4/12/2022 2:09:11 AM
 * Author : dilan
 */ 

// Program for Master Mode
// Check Code2 for Slave Mode Program

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "I2C.h"

uint8_t data=0;


int main(void)
{
	
	TWIInit();
	while(1){
		
		TWIStart();
		TWIWriteAddrs(0x20);
		TWIWriteData(data);
		TWIStop();
		data++;		
		_delay_ms(500);

	}
}
