#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>  //delay library
#include <stdio.h>
#include "USART.h"

float loadCellRead();
uint8_t hx711H=0; //Load Scale High Bits
uint16_t hx711L=0;//Load Scale Low Bits
float hx;
uint16_t hxsend=0;

char data[15];

int main(void)
{
    DDRD|=(1<<7); //Load cell clock pin
    PORTD&=~(1<<7);//Clock pin low
	USART_Init(2400);
	
	DDRD|=(1<<2);
	PORTD&=~(1<<2);
	USART_ClearRX();
	USART_TxStringln("AT");
	_delay_ms(200);
	
	//start setting	
	USART_ClearRX();
	USART_TxStringln("AT+C040");
	USART_ReciveWait("OK",500);
	_delay_ms(100);
//1st C050,2nd C060, 3rd C040

	
	/*USART_ClearRX();
	USART_TxStringln("AT+B2400");
	USART_ReciveWait("OK",500);
	_delay_ms(100);*/
	
	
	
	/*end setting*/
	USART_ClearRX();
	USART_TxStringln("AT");
	_delay_ms(200);
	
	PORTD|=(1<<2);	
	
    while (1) 
    {
		
		hx=loadCellRead(); //3.054
		hxsend=hx*100;//305
		if ((hxsend<10000)&&(hxsend>300))
		{sprintf(data,"ST%05uE",hxsend);//ST00350E
			USART_TxString(data);
		}
		
		
		_delay_ms(100);
    }
}

float loadCellRead(){
	hx711H=0;hx711L=0;  //clear variables
	for(uint8_t i=0;i<8;i++){  // Load cell data high 8 bits
		PORTD|=(1<<7); //Clock pin high
		_delay_us(10);
		if ((PIND&(1<<6))>>6)  //read data pin
		{hx711H|=(1<<(7-i));//set hx 711 varible
		}
		else
		{hx711H&=~(1<<(7-i));
		}
		PORTD&=~(1<<7); //Clock pin low
		_delay_us(5);
	}
	
	
	for(uint8_t i=0;i<16;i++){ // Load cell data low 16 bits
		PORTD|=(1<<7); //Clock pin high
		_delay_us(10);
		if ((PIND&(1<<6))>>6) //read data pin
		{hx711L|=(1<<(15-i));
		}
		else
		{hx711L&=~(1<<(15-i));
		}
		PORTD&=~(1<<7); //Clock pin low
		_delay_us(5);
	}
	
	//hx711L=hx711L>>1; //shift bits
	
	if (hx711H&1)  //bit setup
	{hx711L|=(1<<15);
	}
	else
	{hx711L&=~(1<<15);
	}
	hx711H=hx711H>>1;
	
	return (hx711H*(65536/18029.6))+hx711L/18029.6; //load cell calibration
}