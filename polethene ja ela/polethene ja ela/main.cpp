/*
 * polethene ja ela.cpp
 *
 * Created: 1/10/2022 7:05:41 PM
 * Author : dilan
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "LCDI2C.h"
#include "setjmp.h"

jmp_buf mainstart;

#define sensor 0
#define dec 1
#define inc 2
#define mode 3
#define start 4

#define motor 0
#define mcluch 6
#define mbreak 7

volatile uint16_t timeCount=0;
volatile uint16_t polcount=0;
volatile uint16_t nowTime=0;
volatile uint16_t nowCount=0;

char lcddata[20];

void setValues();

int main(void)
{
    _delay_ms(500);
	
	LcdInit();
	_delay_ms(100);
	LcdInit();
	LcdSetCursor(5,0,"Techart");
	LcdSetCursor(4,1,"Solutions");
	
	setjmp (mainstart);
	nowTime=0;
	nowCount=0;
	_delay_ms(500);
	LcdCommand(LCD_CLEARDISPLAY);
	
	PORTD|=(0b1111<<1);
	DDRB|=(1<<motor);
	DDRD|=(0b11<<6);//cluch break
	PCICR&=~(1<<PCIE2);
		
	TCCR0A|=(1<<WGM01);
	TCCR0B|=(1<<CS01)|(1<<CS00);
	TCNT0=0;
	OCR0A=125;
	TIMSK0&=~(1<<OCIE0A);
	
	
	if ((PIND&(1<<start)))
	{
	} 
	else
	{
		while(1){
			LcdSetCursor(1,0,"Please stop the ");
			LcdSetCursor(5,1,"Machine");
			
			if ((PIND&(1<<start)))
			{LcdCommand(LCD_CLEARDISPLAY);
				_delay_ms(200);
				break;
			}
		}
	}
	
	timeCount= eeprom_read_word((uint16_t*)0);
	polcount=eeprom_read_word((uint16_t*)2);
	if (timeCount>10000)
	{timeCount=9999;
	}
	if (polcount>999)
	{polcount=999;
	}
	
	
	setValues();
	PCICR|=(1<<PCIE2);
	PCMSK2|=(1<<PCINT16);
	sei();
	
    while (1) 
    {
		if (!(PIND&(1<<start)))
		{PORTB|=(1<<motor);
			
		} 
		else
		{PORTB&=~(1<<motor);
			
		}
		
		if ((!(PIND&(1<<inc)))&&(!(PIND&(1<<dec))))
		{
			PORTD&=~(1<<mcluch);
			PORTD&=~(1<<mbreak);
			longjmp(mainstart,1);
		}
		
		if (!(PIND&(1<<mode)))
		{LcdInit();
			sprintf(lcddata,"T:%04ums  C:%03u",timeCount,polcount);
			LcdSetCursor(1,0,lcddata);
			
			uint8_t checktime=0;
			while (1){
				_delay_ms(100);
				
				if (!(PIND&(1<<mode)))
				{checktime++;
				}
				else{
					checktime=0;
					break;
				}
				
				if (checktime>20)//2 sec
				{checktime=0;
					longjmp(mainstart,1);
					
				}
			}
		}
		
		sprintf(lcddata,"%04ums    %03u",nowTime,nowCount);
		LcdSetCursor(3,1,lcddata);
		
		
    }
}

void setValues(){
	LcdInit();
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Set time & count");
	
	uint8_t lcdar[7];
	
	
	lcdar[0]=timeCount/1000;
	lcdar[1]=(timeCount%1000)/100;
	lcdar[2]=(timeCount%100)/10;
	lcdar[3]=timeCount%10;
		
	lcdar[4]=polcount/100;
	lcdar[5]=(polcount%100)/10;
	lcdar[6]=polcount%10;
	
	uint8_t lcdposchar[7]={3,4,5,6,9,10,11};
	uint8_t lcdpos=0;
	
	sprintf(lcddata,"%u%u%u%u  %u%u%u",lcdar[0],lcdar[1],lcdar[2],lcdar[3],lcdar[4],lcdar[5],lcdar[6]);
	LcdSetCursor(3,1,lcddata);
	LcdSetCursor(3,1,"");
	LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKON);  //Blinlk on
	while(1){
		
		if (!(PIND&(1<<dec)))
		{
			if ((lcdar[lcdpos])==0)
			{lcdar[lcdpos]=10;
			}
			lcdar[lcdpos]--;
			
			sprintf(lcddata,"%u%u%u%u  %u%u%u",lcdar[0],lcdar[1],lcdar[2],lcdar[3],lcdar[4],lcdar[5],lcdar[6]);
			LcdSetCursor(3,1,lcddata);
			LcdSetCursor(lcdposchar[lcdpos],1,"");
			_delay_ms(200);
		}
		
		if (!(PIND&(1<<inc)))
		{	lcdar[lcdpos]++;
			
			if ((lcdar[lcdpos])==10)
			{lcdar[lcdpos]=0;
			}
			
			sprintf(lcddata,"%u%u%u%u  %u%u%u",lcdar[0],lcdar[1],lcdar[2],lcdar[3],lcdar[4],lcdar[5],lcdar[6]);
			LcdSetCursor(3,1,lcddata);
			LcdSetCursor(lcdposchar[lcdpos],1,"");
			_delay_ms(200);
		}
		
		if (!(PIND&(1<<mode)))
		{
			lcdpos++;
			if (lcdpos>6)
			{lcdpos=0;
			}
			sprintf(lcddata,"%u%u%u%u  %u%u%u",lcdar[0],lcdar[1],lcdar[2],lcdar[3],lcdar[4],lcdar[5],lcdar[6]);
			LcdSetCursor(3,1,lcddata);
			LcdSetCursor(lcdposchar[lcdpos],1,"");
			_delay_ms(200);
		}
		
		if (!(PIND&(1<<start)))
		{
			LcdCommand(LCD_CLEARDISPLAY);
			LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKOFF);  //Blinlk on
			LcdSetCursor(3,0,"data set");
			
			timeCount=(lcdar[0]*1000)+(lcdar[1]*100)+(lcdar[2]*10)+(lcdar[3]);
			polcount=(lcdar[4]*100)+(lcdar[5]*10)+(lcdar[6]);
			sprintf(lcddata,"%04u  %03u",timeCount,polcount);
			LcdSetCursor(3,1,lcddata);
			eeprom_write_word((uint16_t*)0,timeCount);
			eeprom_write_word((uint16_t*)2,polcount);
			_delay_ms(1000);
			break;
			
		}
	}
	
	LcdCommand(LCD_CLEARDISPLAY);
	sprintf(lcddata,"T:%04ums  C:%03u",timeCount,polcount);
	LcdSetCursor(1,0,lcddata);
}

ISR(PCINT2_vect){
	if (PIND&(1<<sensor))
	{PORTD|=(1<<mcluch);
	PORTD&=~(1<<mbreak);
	TIMSK0|=(1<<OCIE0A);
	} 
	else
	{PORTD|=(1<<mbreak);
	PORTD&=~(1<<mcluch);
	TIMSK0&=~(1<<OCIE0A);
	nowTime=0;
	
	}
}

ISR(TIMER0_COMPA_vect){
	
	TCNT0=0;
	
	nowTime++;
	
	if (nowTime>timeCount)
	{nowTime=0;
		PORTD|=(1<<mbreak);
		PORTD&=~(1<<mcluch);
		TIMSK0&=~(1<<OCIE0A);
		nowCount++;
		if (nowCount>=polcount)
		{PCICR&=~(1<<PCIE2);
		}
	}
}