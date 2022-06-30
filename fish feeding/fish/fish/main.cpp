/*
 * fish.cpp
 *
 * Created: 1/9/2021 9:49:45 AM
 * Author : Dilan
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "USART.h"
#include "I2C.h"

#define StepDDR DDRB
#define StepPORT PORTB
#define stepM PINB0

uint8_t hourLimit= 17;
uint8_t minLimit = 30;
uint8_t secLimit =00;

void StepIn(uint8_t steps){
	
	for(uint8_t i=0;i<steps;i++){
		StepPORT|=(1<<stepM);
		StepPORT|=(1<<5);
		_delay_ms(15);
		StepPORT&=~(1<<stepM);
		StepPORT&=~(1<<5);
		_delay_ms(15);
		
	}
	
}

void setTime(uint8_t hSet,uint8_t mSet,uint8_t sSet){
	TWIStart();
	TWIWriteAddrs(104,0);
	TWIWriteData(0x02);
	TWIWriteData(((hSet/10)<<4)|(hSet%10));
	TWIStop();
	
	TWIStart();
	TWIWriteAddrs(104,0);
	TWIWriteData(0x01);
	TWIWriteData(((mSet/10)<<4)|(mSet%10));
	TWIStop();
	
	TWIStart();
	TWIWriteAddrs(104,0);
	TWIWriteData(0x00);
	TWIWriteData(((sSet/10)<<4)|(sSet%10));
	TWIStop();
	
	
}

int main(void)
{
    USART_Init(9600);
	USART_TxStringln("Statred");
	
	/*stepper motor setup*/
	StepDDR|=(1<<stepM);
	StepDDR|=(1<<5);
	
	DDRB&=~(1<<7);
	PORTB|=(1<<7);	
	TWIInit();
	//setTime(14,48,55);
	
    while (1) 
    {
		
		TWIStart();/* Start I2C with device write address */
		TWIWriteAddrs(104,0); //Return 24 mean device found, return mean no device
		TWIWriteData(0);  //Register address
		TWIStart();
		TWIWriteAddrs(104,1); //Write bit
		uint16_t h=TWIReadACK();
		h|=TWIReadACK()<<8;
		h|=TWIReadNACK()<<16;
		TWIStop();
		
		uint8_t sec=((0x00F0&h)>>4)*10+(0x000F&h);  /*seonds first num and sec num*/
		uint8_t min=((0xF000&h)>>12)*10+((0x0F00&h)>>8);  /*min first num and sec num*/
		
		TWIStart();/* Start I2C with device write address */
		TWIWriteAddrs(104,0); //Return 24 mean device found, return mean no device
		TWIWriteData(2);  //Register address
		TWIStart();
		TWIWriteAddrs(104,1); //Write bit
		uint8_t lh=TWIReadNACK();
		TWIStop();
		
		uint8_t hour=((0b00110000&lh)>>4)*10+(0x0F&lh);  /*seonds first num and sec num*/
		
		if ((((hour==hourLimit)&&(min==minLimit)&&(sec==secLimit)))||((PINB&(1<<7))==0))
		{uint8_t eeread= eeprom_read_byte((uint8_t*)0);
			if (eeread==0)
			{StepIn(7);
				eeprom_write_byte((uint8_t*)0,1);
			}
			else if (eeread==1)
			{StepIn(7);
				eeprom_write_byte((uint8_t*)0,2);
			}
			else{
				StepIn(6);
				eeprom_write_byte((uint8_t*)0,0);
			}
			
			_delay_ms(1500);
		}
		
			
		USART_TxNumber(hour,10);
		USART_TxString(":");
		USART_TxNumber(min,10);
		USART_TxString(":");
		USART_TxNumber(sec,10);
		USART_TxString("\n");
		
		_delay_ms(100);
		
    }
}

