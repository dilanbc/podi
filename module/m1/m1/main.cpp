
#define F_CPU 8000000//xtal speed
#include <avr/io.h>
#include <util/delay.h>//delay library
#include <avr/eeprom.h>//eeprom memory library
#include <stdio.h>  //library for sprintlf function
#include "USART.h"// Serial communication libabry
#include "LCDI2C.h"// lcd libary

#define LENGTH 82 //20
volatile uint16_t TimerCal=0;
 uint16_t ultra=0;
 
 char txword[15];// lcd data
 uint8_t keyRead();//keypad read function
void EnterID();//ID card
void EnterPhone();//phone number part
int main(void)
{   /*Keypad_Init*/
	DDRB=0b00000111;	
	
   /*Ultrasonic Timer Part*/
	 DDRD|=(1<<2); //D2 as output
	TCCR0|=(1<<WGM01);//Enable Compare match mode
	TCCR0|=(1<<CS01);//Start timer  prescaler =8
	TCNT0=0;
	OCR0=10;//answere
	/*register value= time*(clock speed/prescale)
	register value=0.00001S*(8000000/8)
	register value=10*/
	TIMSK|=(1<<OCIE0);//enable timer Compare inturrept
	
	sei();//global int enable
	
	USART_Init(9600);  //Start Serial Communication
	USART_TxStringln("Started");
	LcdInit(); //Start LCD
	LcdSetCursor(4,0,"WELCOME");
	
    while (1) 
    {
		/*Ultrasonic Part*/
		PORTD&=~(1<<2);// trigger signal low
		_delay_us(50);
		PORTD|=(1<<2);// trigger signal high
		_delay_us(50);
		PORTD&=~(1<<2);// trigger signal low
		while(!(PIND&(1<<3)))// waiting for rising edge
		TimerCal=0;//reset timer
		while((PIND&(1<<3)))// waiting for falling edge
		ultra=TimerCal;//
		sprintf(txword," %u \r\n ",ultra);
		USART_TxStringln(txword);
		
		if (ultra<LENGTH)
		{LcdSetCursor(0,0,"Enter Your ID");
			EnterID();
			LcdSetCursor(0,0,"Enter Your T.P");
			EnterPhone();
			
			LcdSetCursor(4,0,"WELCOME");
			_delay_ms(3000);
		}
		
		
		_delay_ms(100);
    }
}

ISR(TIMER0_COMP_vect){//timer inturrept  0.0001S
	TimerCal++;
	TCNT0=0;
	
}

void EnterID(){
	uint8_t pos=0;
	char lcddNum[5];
	LcdSetCursor(0,1,"");
	LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKON); 
	
	while(1){
		
		uint8_t readKey=keyRead();
		if (readKey<10)
		{ USART_TxNumber(readKey);
			sprintf(lcddNum,"%u",readKey);
			LcdSetCursor(pos,1,lcddNum);
			pos++;
			readKey=12;
			_delay_ms(100);
		}
		else if ((readKey==10)&&(pos>0))
		{pos--;
			LcdSetCursor(pos,1," ");
			LcdSetCursor(pos,1,"");
			readKey=12;
			_delay_ms(100);
		}
		else if ((readKey==11)&&(pos>0))
		{
			LcdCommand(LCD_CLEARDISPLAY);
			LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKOFF); 
			LcdSetCursor(0,0,"ID Number Saved");
			_delay_ms(1000);
			LcdCommand(LCD_CLEARDISPLAY);
			
			break;
		}
		
		
	}
}

void EnterPhone(){
	
	uint8_t pos=0;
	char lcddNum[5];
	LcdSetCursor(0,1,"");
	LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKON);
	
	while(1){
		
		uint8_t readKey=keyRead();
		if (readKey<10)
		{ USART_TxNumber(readKey);
			sprintf(lcddNum,"%u",readKey);//setup character
			LcdSetCursor(pos,1,lcddNum);//show number
			eeprom_write_byte((uint8_t*)pos,readKey);//write in eeprom
			pos++;
			readKey=12;
			_delay_ms(100);
		}
		else if ((readKey==10)&&(pos>0))
		{pos--;
			LcdSetCursor(pos,1," ");
			LcdSetCursor(pos,1,"");//blink
			readKey=12;
			_delay_ms(100);
		}
		else if ((readKey==11)&&(pos>9))
		{
			LcdCommand(LCD_CLEARDISPLAY);
			LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKOFF);
			LcdSetCursor(0,0,"Number Saved");
			_delay_ms(1000);
			LcdCommand(LCD_CLEARDISPLAY);
			break;
		}
		
		
	}
	
	
}

uint8_t keyRead(){
	uint8_t keypad[4][3]={{1,2,3},{4,5,6},{7,8,9},{10,0,11}};//keypad array
	
	uint8_t key=12;
	for(uint8_t i=0;i<3;i++){
		PORTB=(1<<i);
		if (PINB&(1<<3)) //check PINB3 high???
		{	
			return keypad[0][i];
		} 
		else if(PINB&(1<<4)){//check PINB4 high???
		return keypad[1][i];	
		}
		else if(PINB&(1<<5))//check PINB5 high???
		{
			return keypad[2][i];
		}
		else if(PINB&(1<<6))//check PINB6 high???
		{
			return keypad[3][i];
		}
		_delay_ms(50);
	}
	return 12;
}

