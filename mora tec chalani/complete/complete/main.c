#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "LCD.h"
#include "USART.h"

const uint8_t kaypad[4][4]={{7,8,9,10},{4,5,6,11},{1,2,3,12},{13,0,14,15}};
uint8_t getkeyNum();
char lcddata[20];
uint8_t keyout;


void EnterID();
void EnterAge();
void EnterPhone();
uint16_t PatID=0;
uint16_t PatAge=0;
uint16_t PhoneH=0;
uint16_t PhoneL=0;

uint8_t Pressure= 22;
uint8_t Tempurature=36;

uint8_t user=0;



int main(void)
{
	LcdInit();
	USART_Init(2400);
	DDRD|=(0b1111<<3);
	//eeprom_write_byte((uint8_t*)0,0);
	PORTC|=(1<<2);
	user=eeprom_read_byte((uint8_t*)0);
	EnterID();
	EnterAge();
	EnterPhone();
	
	
	PatID=eeprom_read_word((uint16_t*)((user*9)+1));
	PatAge=eeprom_read_word((uint16_t*)((user*9)+3));
	PhoneH=eeprom_read_word((uint16_t*)((user*9)+5));
	PhoneL=eeprom_read_word((uint16_t*)((user*9)+7));
	
	//sprintf(lcddata,"%u %u",PatID,PatAge);
	//LcdSetCursor(0,0,lcddata);
	
//	sprintf(lcddata,"07%u%u",PhoneH,PhoneL);
	//LcdSetCursor(0,1,lcddata);
	
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Place Finger");
	
	
	while (1)
	{
		if (!(PINC&(1<<2)))
		{	LcdCommand(LCD_CLEARDISPLAY);
			break;
		}
	}
	
	sprintf(lcddata,"Tempura %02u",Tempurature);
	LcdSetCursor(0,0,lcddata);
	sprintf(lcddata,"Pressure %02u",Pressure);
	LcdSetCursor(0,1,lcddata);
	_delay_ms(1000);
	LcdCommand(LCD_CLEARDISPLAY);
	
	USART_TxStringln("AT");
	_delay_ms(500);
	USART_TxStringln("AT");
	_delay_ms(500);
	USART_TxStringln("AT+CMGF=1");
	_delay_ms(500);
	USART_TxStringln("AT+CMGS=\"+947112345612\"");
	_delay_ms(200);
	user++;
	eeprom_write_byte((uint8_t*)0,user);
	
	
	sprintf(lcddata,"Tempura %02u",Tempurature);
	USART_TxStringln(lcddata);
	sprintf(lcddata,"Pressure %02u",Pressure);
	USART_TxStringln(lcddata);
	
	sprintf(lcddata,"ID -%u Age %u",PatID,PatAge);
	USART_TxStringln(lcddata);
	
	sprintf(lcddata,"Number- 07%u%u",PhoneH,PhoneL);
	USART_TxStringln(lcddata);
	
	_delay_ms(200);
	USART_Transmit(26);
	_delay_ms(200);
	_delay_ms(1000);
	
	
	LcdSetCursor(0,0,"Waiting . .");
	 while (1)
	 {
		 if ((rchar>64)&&(rchar<68))
		 {	
			break;			
			
		 }
	 }
	 PORTD|=(1<<5);
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Message Received");
	switch(rchar){
		case 65:LcdSetCursor(0,1,"Message A"); break;
		case 66:LcdSetCursor(0,1,"Message B"); break;
		case 67:LcdSetCursor(0,1,"Message C"); break;
	}
	_delay_ms(5000);
	PORTD&=~(1<<5);
	
}

uint8_t getkeyNum(){
	DDRA=0b00001111;
	PORTA=0b11110000;
	uint8_t getx;
	uint8_t gety;
	while(1){
		
		uint8_t pin=(~(PINA|0x0F));
		if (pin)
		{
			
			
			switch(pin){
				
				
				case (1<<4) :getx=0; break;
				case (1<<5) :getx=1;break;
				case (1<<6) :getx=2;break;
				case (1<<7) :getx=3;break;
				
				
			} //switch
			DDRA=0b0;
			PORTA=0b0;
			
			DDRA=0b11110000;
			PORTA=0b00001111;
			
			pin=~(PINA|0xF0);
			
			switch(pin){
				case (1<<0) :gety=0;break;
				case (1<<1) :gety=1;break;
				case (1<<2) :gety=2;break;
				case (1<<3) :gety=3;break;
				
			} //switch
			
			DDRA=0b00001111;
			PORTA=0b11110000;
			
			
			return kaypad[gety][getx];
			
		}//if
		
		
		
	}//while 1
}


void EnterID(){
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Enter ID");
	_delay_ms(500);
	uint8_t pos=0;
	uint8_t numbers[4]={0,0,0,0};
	uint8_t posData[4][2]={{0,9},{1,9},{2,9},{3,9}};


	sprintf(lcddata,"%u%u%u%u",numbers[0],numbers[1],numbers[2],numbers[3]);
	LcdSetCursor(0,1,lcddata);
	LcdSetCursor(pos,1,"");
	LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKON);

	while(1){
		uint8_t keyout=getkeyNum();
		
		if (keyout<=posData[pos][1])
		{
			numbers[pos]=keyout;
			sprintf(lcddata,"%u%u%u%u",numbers[0],numbers[1],numbers[2],numbers[3]);
			LcdSetCursor(0,1,lcddata);
			pos++;
			if (pos>3)
			{pos=0;
			}
			LcdSetCursor(posData[pos][0],1,"");
			_delay_ms(200);
		}
		
		else if (keyout==14)
		{
			LcdCommand(LCD_CLEARDISPLAY);
			LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKOFF);
			LcdSetCursor(0,0,"ID Added");
			PatID=(numbers[0]*1000)+(numbers[1]*100)+(numbers[2]*10)+numbers[3];
			eeprom_write_word((uint16_t*)((user*9)+1),PatID);
			_delay_ms(500);
			break;
		}
	}
}


void EnterAge(){
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Enter Age");
	_delay_ms(500);
	uint8_t pos=0;
	uint8_t numbers[3]={0,0,0};
	uint8_t posData[3][2]={{0,9},{1,9},{2,9}};


	sprintf(lcddata,"%u%u%u",numbers[0],numbers[1],numbers[2]);
	LcdSetCursor(0,1,lcddata);
	LcdSetCursor(pos,1,"");
	LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKON);

	while(1){
		uint8_t keyout=getkeyNum();
		
		if (keyout<=posData[pos][1])
		{
			numbers[pos]=keyout;
			sprintf(lcddata,"%u%u%u",numbers[0],numbers[1],numbers[2]);
			LcdSetCursor(0,1,lcddata);
			pos++;
			if (pos>2)
			{pos=0;
			}
			LcdSetCursor(posData[pos][0],1,"");
			_delay_ms(200);
		}
		
		else if (keyout==14)
		{
			LcdCommand(LCD_CLEARDISPLAY);
			LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKOFF);
			LcdSetCursor(0,0,"Age Added");
			PatAge=(numbers[0]*100)+(numbers[1]*10)+numbers[2];
			eeprom_write_word((uint16_t*)((user*9)+3),PatAge);
			_delay_ms(500);
			break;
		}
	}
}

void EnterPhone(){
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Enter PhoneNum.");
	_delay_ms(500);
	uint8_t pos=0;
	uint8_t numbers[8]={0,0,0,0,0,0,0,0};
	uint8_t posData[8][2]={{2,9},{3,9},{4,9},{5,9},{6,9},{7,9},{8,9},{9,9}};


	sprintf(lcddata,"07%u%u%u%u%u%u%u%u",numbers[0],numbers[1],numbers[2],numbers[3],numbers[4],numbers[5],numbers[6],numbers[7]);
	LcdSetCursor(0,1,lcddata);
	LcdSetCursor(pos,1,"");
	LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKON);
	LcdSetCursor(posData[pos][0],1,"");

	while(1){
		uint8_t keyout=getkeyNum();
		
		if (keyout<=posData[pos][1])
		{
			numbers[pos]=keyout;
			sprintf(lcddata,"07%u%u%u%u%u%u%u%u",numbers[0],numbers[1],numbers[2],numbers[3],numbers[4],numbers[5],numbers[6],numbers[7]);
			LcdSetCursor(0,1,lcddata);
			pos++;
			if (pos>7)
			{pos=0;
			}
			LcdSetCursor(posData[pos][0],1,"");
			_delay_ms(200);
		}
		
		else if (keyout==14)
		{
			LcdCommand(LCD_CLEARDISPLAY);
			LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKOFF);
			LcdSetCursor(0,0,"Phone Num. Added");
			PhoneH=(numbers[0]*1000)+(numbers[1]*100)+(numbers[2]*10)+numbers[3];
			PhoneL=(numbers[4]*1000)+(numbers[5]*100)+(numbers[6]*10)+numbers[7];
			eeprom_write_word((uint16_t*)((user*9)+5),PhoneH);
			eeprom_write_word((uint16_t*)((user*9)+7),PhoneL);
			_delay_ms(500);
			break;
		}
	}
}