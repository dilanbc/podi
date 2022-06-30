#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include "LCD.h"




uint8_t hx711H=0; //Load Scale High Bits
uint16_t hx711L=0;//Load Scale Low Bits
volatile uint16_t TimerCal=0;// variable for collect echo data
float loadCellRead();
#define Loadcell_data 1
#define Loadcell_clk 0


char lcddata[20];


int main(void)
{
	LcdInit();
	
	DDRC|=(1<<Loadcell_clk); //Load cell clock pin
	PORTC&=~(1<<Loadcell_clk);//Clock pin low
	_delay_ms(100);
	DDRD|=(1<<3);
	while (1)
	{	
			float hx=loadCellRead();
			sprintf(lcddata," %.3f  ",hx);
			LcdSetCursor(0,0,lcddata);
			_delay_ms(100);
			if (PIND&(1<<2))
			{PORTD|=(1<<3);
			}
		else{
			PORTD&=~(1<<3);
		}
	}
}


float loadCellRead(){
	hx711H=0;hx711L=0;  //clear variables
	for(uint8_t i=0;i<8;i++){  // Load cell data high 8 bits
		PORTC|=(1<<Loadcell_clk); //Clock pin high
		_delay_us(10);
		if ((PINC&(1<<Loadcell_data))>>Loadcell_data)  //read data pin
		{hx711H|=(1<<(7-i));//set hx 711 varible
		}
		else
		{hx711H&=~(1<<(7-i));
		}
		PORTC&=~(1<<Loadcell_clk); //Clock pin low
		_delay_us(5);
	}
	
	
	for(uint8_t i=0;i<16;i++){ // Load cell data low 16 bits
		PORTC|=(1<<Loadcell_clk); //Clock pin high
		_delay_us(10);
		if ((PINC&(1<<Loadcell_data))>>Loadcell_data) //read data pin
		{hx711L|=(1<<(15-i));
		}
		else
		{hx711L&=~(1<<(15-i));
		}
		PORTC&=~(1<<Loadcell_clk); //Clock pin low
		_delay_us(5);
	}
	
	hx711L=hx711L>>1; //shift bits
	
	if (hx711H&1)  //bit setup
	{hx711L|=(1<<15);
	}
	else
	{hx711L&=~(1<<15);
	}
	hx711H=hx711H>>1;
	
	return (hx711H*(65536/18029.6))+hx711L/18029.6; //load cell calibration
}