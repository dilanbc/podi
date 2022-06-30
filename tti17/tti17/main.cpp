#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <string.h>
#include "setjmp.h"
#include "USART.h"

jmp_buf mainstart;

void mainFunctionInit();
bool checkGSM();
void setupGSM();

char*apn="mobitel";
char lengthcal[40];//string memory
char reqvest[120];
char*postserver="http://smartmeterlk.000webhostapp.com/write.php";

bool uploaddataPOST(float unit,uint8_t watt);
bool readdataPOST();
void InitADC();

volatile bool readan=0;
volatile uint16_t analogRead=0;


volatile uint16_t tempNum;
volatile uint16_t counter=0;
volatile uint16_t lastsave=0;
volatile uint16_t finale=0;

float unit=0;
uint8_t watt=0;

int main(void)
{	_delay_ms(500);
	DDRB|=(1<<0);
	DDRD|=(1<<2);
	unit=eeprom_read_float((float*)1);
	if (eeprom_read_byte((uint8_t*)0))
	{PORTB|=(1<<0);
	}
	else
	{PORTB&=~(1<<0);
	}
	
	 setjmp (mainstart);
	 
	mainFunctionInit();
   
    _delay_ms(500);
    sei();
    
    if (!checkGSM())
    {longjmp(mainstart,1);
    }
    setupGSM();
	
    while (1) 
    {
		if (!uploaddataPOST(unit,watt))
		{longjmp(mainstart,1);
		}
		
    }
}

void mainFunctionInit(){
	PORTD|=(1<<2);
	_delay_ms(200);
	PORTD&=~(1<<2);
	_delay_ms(200);
	PORTD|=(1<<2);
	USART_Init(9600);
	USART_ClearRX();
	InitADC();
	
}

void InitADC(){
	ADMUX|=(1<<REFS1)|(1<<REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	if (readan)
	{ADMUX=(ADMUX&0xF0)|(readan);
	} 
	else
	{ADMUX=(ADMUX&0xF0)|(readan);
	}
	ADCSRA|=(1<<ADSC);
	
}

ISR(ADC_vect){
	analogRead=ADC;
	if (readan)//power
	{
		if (analogRead<500)
		{eeprom_write_byte((uint8_t*)0,(PORTB&0b1));
			eeprom_write_float((float*)1,unit);
			_delay_ms(1000);
			PORTB|=(1<<0);
		}
	} 
	else//unit
	{
		
		
		
		
		tempNum=analogRead;//30
		if (tempNum>lastsave)
		{lastsave=tempNum;
		}
		counter++;
		if (counter>2000)
		{finale=lastsave;
			lastsave=0;
			counter=0;
			if (finale>17)
			{watt=finale;
				unit+=watt/180000000;
			}
			else
			{watt=0;
			}
			
			
		}
	}
	
	readan=!readan;
	if (readan)
	{ADMUX=(ADMUX&0xF0)|(readan);
	}
	else
	{ADMUX=(ADMUX&0xF0)|(readan);
	}
	ADCSRA|=(1<<ADSC);
	
}

bool checkGSM(){
	PORTD&=~(1<<2);
	_delay_ms(500);
	PORTD|=(1<<2);
	_delay_ms(1000);
	for(uint8_t i=0;i<20;i++){
		USART_TxStringln((char*)"AT");
		if (USART_ReciveWait((char*)"OK",1000))
		{USART_ClearRX();
			break;
		}
		if (i==19)
		{return 0;
		}
	}
	
	int gsm;//gsm signal calculation
	
	for(uint8_t i=0;i<20;i++){
		USART_TxStringln("AT+CSQ");  //signal strength
		_delay_ms(500);
		strncpy(lengthcal,strstr(rxvalue,"CSQ: ")+5,2);
		gsm=atoi(lengthcal);
		if (gsm>12)
		{USART_ClearRX();
			return 1;
		}
		else{
			USART_ClearRX();
			_delay_ms(1000);
		}
		USART_ClearRX();
	}
	return 0;
}

void setupGSM(){
	//******SMS Functions
	USART_TxStringln((char*)"AT+CMGF=1");
	USART_ReciveWait((char*)"OK",1000,1);
	_delay_ms(200);
	USART_ClearRX();
	
	USART_TxStringln((char*)"AT+CNMI=1,2,0,0,0");
	USART_ReciveWait((char*)"OK",1000,1);
	_delay_ms(200);
	
	USART_TxStringln("AT+SAPBR=3,1,Contype,GPRS");
	USART_ReciveWait("OK",1000,1);
	_delay_ms(200);
	
	sprintf(lengthcal,"AT+SAPBR=3,1,APN,%s",apn);
	USART_TxStringln(lengthcal);
	USART_ReciveWait("OK",1000);
	_delay_ms(200);
	USART_ClearRX();
}

bool uploaddataPOST(float unit,uint8_t watt){
	
	bool returnValue=0;
	
	USART_TxStringln("AT+SAPBR=2,1");
	USART_ReciveWait("OK",3000);
	_delay_ms(25);
	
	if (USART_ReciveWait("0.0.0.0",100))
	{USART_TxStringln("AT+SAPBR =1,1");
	_delay_ms(25);
	USART_ClearRX();
	} 
	
	
	USART_ClearRX();
	
	
	
	
	USART_TxStringln("AT+SAPBR=2,1");
	USART_ReciveWait("OK",3000);
	_delay_ms(25);
	USART_ClearRX();
	
	
	USART_TxStringln("AT+HTTPINIT");
	_delay_ms(25);
	USART_ClearRX();
	
	
	USART_TxStringln("AT+HTTPPARA=CID,1");
	USART_ReciveWait("OK",1000);
	_delay_ms(25);
	USART_ClearRX();
	
	
	sprintf(reqvest,"AT+HTTPPARA=URL,%s",postserver);
	USART_TxStringln(reqvest);
	USART_ReciveWait("OK",1000);
	_delay_ms(25);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
	USART_ReciveWait("OK",1000);
	_delay_ms(25);
	USART_ClearRX();
	
	
	sprintf(reqvest,"unitw=%.2f&wattw=%u",unit,watt);
	
	sprintf(lengthcal,"AT+HTTPDATA=%d,10000",strlen(reqvest));
	USART_TxStringln(lengthcal);
	USART_ReciveWait("OK",1000);
	_delay_ms(25);
	USART_ClearRX();
	
	USART_TxStringln(reqvest);
	USART_ReciveWait("OK",10000);
	_delay_ms(25);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPACTION=1");
	USART_ReciveWait("+HTTPACTION:",10000);
	_delay_ms(25);
	USART_ClearRX();
	

	
	USART_TxStringln("AT+HTTPREAD");
	if (USART_ReciveWait("unitADwattAD",5000))
	{	
		uint8_t checkS=0;
		strncpy(lengthcal,strstr(rxvalue,"unitADwattAD")+12,1);
		checkS=atoi(lengthcal);
		if (checkS)
		{PORTB|=(1<<0);
		} 
		else
		{PORTB&=~(1<<0);
		}
		
		returnValue=1;
	} 
		
	_delay_ms(25);
	USART_ClearRX();
	/*
	USART_TxStringln("AT+HTTPTERM");
	USART_ReciveWait("OK",1000);
	_delay_ms(25);
	USART_ClearRX();*/
	return returnValue;
	
}


