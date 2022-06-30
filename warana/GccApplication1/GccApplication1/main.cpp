/*
 * GccApplication1.cpp
 *
 * Created: 9/8/2020 3:31:59 PM
 * Author : Dilan
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "SoftUART.h"
#include "LCDI2C.h"

uint16_t ReadADC(uint8_t ADCchannel);
void InitADC();



uint8_t timei=0;
bool swMode[2]={0,0};
int8_t swTime[2]={0,0};
bool diMode;	

int main(void){
    DDRB|=(1<<5);
	DDRD=(1<<6)|(1<<7);
	
	
	InitADC();
	USART_Init(9600);
	_delay_ms(500);
	SoftUART_Init();
	LcdInit();
	LcdSetCursor(0,4,"WELCOME");
	_delay_ms(3000);
	USART_TxStringln("AT");
	_delay_ms(500);
	USART_TxStringln("AT");
	SoftUSART_tx_str("Started");
	_delay_ms(500);	
	if (USART_ReciveWait("OK",2000)==1)                 //Command and time out
	{SoftUSART_tx_strln("GSM OK");
		LcdSetCursor(0,4,"Started");
		_delay_ms(500);
	}
	else
	{SoftUSART_tx_strln("GSM not found");
		LcdSetCursor(0,0,"GSM Not Found");
		while(1);
	}
	
	///////////////////////////////////////////////////////
	_delay_ms(1000);
/*
	USART_TxStringln("AT+CPIN?");
	SoftUSART_tx_str("Started");
	_delay_ms(500);
	if (USART_ReciveWait("SMS Ready",10000)==1)                 //Command and time out
	{SoftUSART_tx_strln("GSM OK");
		LcdCommand(LCD_CLEARDISPLAY);
		LcdSetCursor(0,5,"Network");
		LcdSetCursor(1,4,"Connected");
		_delay_ms(500);
	}
	else
	{SoftUSART_tx_strln("Network Problem");
		SoftUSART_tx_strln("GSM OK");
		LcdCommand(LCD_CLEARDISPLAY);
		LcdSetCursor(0,0,"Network Problem");
		while(1);
	}*/
	//////////////////////////////////////////////////////
	_delay_ms(200);
	USART_ClearRX();
	USART_TxStringln("AT+CMGF=1");
	USART_ReciveWait("OK",1000);
	
	_delay_ms(200);
	USART_ClearRX();
	USART_TxStringln("AT+CNMI=1,2,0,0,0");
	USART_ReciveWait("OK",1000);
	SoftUSART_tx_strln("loop Started");
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(1,0,"SW0");
	LcdSetCursor(1,8,"SW1");
	LcdSetCursor(0,0,"Current");
	
	OCR1A=15624;
	TCCR1B|=(1<<WGM12);
	TIMSK|=(1<<OCIE1A);
	TCCR1B|=(1<<CS12)|(1<<CS10);
	
	
	
	swMode[0]=1;
	swMode[1]=1;
	PORTD|=(1<<6)|(1<<6);
    while (1) 
    {	
		
		
		float reading= -22+((ReadADC(0)/921.6)*40);
		char  newbf[5];
		
		dtostrf(reading,4,3,newbf);
		LcdSetCursor(0,8,newbf);
		_delay_ms(500);
		

		diMode=!diMode;
		if (diMode==0)
		{LcdSetCursor(1,4,"   ");
			_delay_ms(100);
			if (swMode[0]==1)
			{LcdSetCursor(1,5,"ON");
			} 
			else
			{LcdSetCursor(1,4,"OFF");
			}
			_delay_ms(100);
		LcdSetCursor(1,12,"   ");
		_delay_ms(100);
		
		if (swMode[1]==1)
		{LcdSetCursor(1,13,"ON");
		}
		else
		{LcdSetCursor(1,12,"OFF");
		}
		_delay_ms(100);
		} 
		else
		{
			LcdSetCursor(1,4,"   ");
			_delay_ms(100);
			char dis [3];
			itoa(swTime[0],dis,10);
			LcdSetCursor(1,4,dis);
			_delay_ms(100);
			
			LcdSetCursor(1,12,"   ");
			_delay_ms(100);
			
			itoa(swTime[1],dis,10);
			LcdSetCursor(1,12,dis);
			_delay_ms(100);
			
			
		}
		
		
		if(USART_ReciveWait("SW",200)){
			SoftUSART_tx_strln("Msg recived");
			
			char filtere[15];
			strncpy(filtere,strstr(rxvalue,"SW"),15);
			strupr(filtere);
			SoftUSART_tx_strln(filtere);
			
			char rlnumC[1];
			strncpy(rlnumC,filtere+2,1);
			bool rlNum=atoi(rlnumC);
			
			
			char rlStC[1];
			strncpy(rlStC,filtere+4,1);
			int rlst = strcmp(rlStC,"A");
						
			char RxnumC[3];
			strncpy(RxnumC,strstr(filtere,"T")+1,3);
			int timelimit=atoi(RxnumC);
			
			
			
			
			
			if ((rlst==5)||(rlst==13))
			{	
				
				bool rlSW=0;
				if (rlst==13)
				{rlSW=1;
				}
				
				swMode[rlNum]=rlSW;
				swTime[rlNum]=timelimit;
				
				
				
				
				SoftUSART_TxNumber(rlNum);
				SoftUSART_tx_strln("");
				SoftUSART_TxNumber(rlst);
				SoftUSART_tx_strln("");
				SoftUSART_TxNumber(timelimit);
			}
			
			USART_ClearRX();
		}
		/*SoftUSART_TxNumber(swMode[0]);
		SoftUSART_TxNumber(swMode[1]);
		//SoftUSART_TxNumber(timei);
		SoftUSART_tx_strln("");*/
    }
}
void InitADC()
{
	// Select Vref=AVcc
	ADMUX |= (1<<REFS0);
	//set prescaller to 128 and enable ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}
uint16_t ReadADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	return ADC;
}

ISR(TIMER1_COMPA_vect){
	
	TCNT1=0;
	timei++;
	
	

	
	
	if (timei>59)
	{timei=0;
		if (swTime[0]>0)
		{swTime[0]--;
		}
		else{
			if (swMode[0]==0)
			{PORTD&=~(1<<6);
			}
			else{PORTD|=(1<<6);}
	}
	
	
	
	
	if (swTime[1]>0)
	{swTime[1]--;
	}
	else{
		if (swMode[1]==0)
		{PORTD&=~(1<<7);
		}
		else{PORTD|=(1<<7);}
	}
		
	}
	
	
		
		if (swTime[0]==0)
		{
			if (swMode[0]==0)
			{PORTD&=~(1<<6);
			}
			else{PORTD|=(1<<6);}
		}
		
		
		if (swTime[1]==0)
		{
			if (swMode[1]==0)
			{PORTD&=~(1<<7);
			}
			else{PORTD|=(1<<7);}
	}
		
		
		
		
		
		
		
	
	

}

