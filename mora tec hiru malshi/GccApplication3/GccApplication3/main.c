#define F_CPU 20000000
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "USART.h"
#include "LCDI2C.h"

unsigned char UART_RxChar();
uint8_t i=0;
char lcddata[20];

int main(void)
{
	USART_Init(9600);
	LcdInit(0x27);
	USART_TxStringln("AT");
	
	LcdSetCursor(0,0,rxvalue);
	USART_ClearRX();
	
	//USART_getLocation();
	
	longitude=10.58;
	laditute=20.12;
	
	sprintf(lcddata,"%2.3f %2.3f",longitude,laditute);
	LcdSetCursor(0,0,lcddata);
	
	while (1)
	{
		
		sprintf(lcddata,"%c",(((PINB&0b1110)>>1)+64));
		LcdSetCursor(0,1,lcddata);
		
		
	}
}

