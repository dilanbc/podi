/*
 * google.cpp
 *
 * Created: 4/28/2021 6:28:43 PM
 * Author : Kokum
 */ 


  
#define F_CPU 8000000UL
#include <avr/io.h>

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "setjmp.h"
#include "stdio.h"
#include "USART.h"
#include "oled_ssd1306.h"


char tempcharacter;
char reqvest[120];
char lengthcal[40];
char*postserver="https://smartmeterlk.000webhostapp.com/write.php";


int gsm;

char*apn="hutch3g";

volatile uint16_t tempNum;
volatile float meterReading=0;
volatile bool anlogSelect=0;//0 for chckV 1 for Currentvoltage
volatile uint32_t TimeCounter=0;
volatile uint16_t batteryLvel;
volatile uint16_t currentLevel;
volatile uint16_t tempset[48];
volatile float maxVal=0;
volatile float recentMax;
jmp_buf mainstart;






void mainFunctionInit();
bool checkGSM();
bool setupGSM();
uint16_t ReadADC(uint8_t ADCchannel);
void InitADC();
bool uploaddataPOST();



int main(void)
{	setjmp (mainstart);
	DDRD|=(1<<3);
	PORTD&=~(1<<3);
	_delay_ms(500);
	PORTD|=(1<<3);
	
	sei();
	mainFunctionInit();
	/*oled_gotoxy(5,5);
	oled_font_size(0);
	oled_write("Smart Meter");*/
	if (!checkGSM())
	{longjmp(mainstart,1);
	}
	setupGSM();
	
	
    
	
    while (1) 
    {
		
		uploaddataPOST();
	
		_delay_ms(10000);
    }
}

void mainFunctionInit(){
	USART_Init(9600);
	USART_ClearRX();
	InitADC();
	//eeprom_write_float((float*)0,0);
	//eeprom_write_byte((uint8_t*)5,0);
	meterReading=eeprom_read_float(0);
	DDRB|=(1<<0);
	if (eeprom_read_byte((uint8_t*)5))
	{PORTB|=(1<<0);
	} 
	else
	{PORTB&=~(1<<0);
	}
	
	
}
void InitADC()  //meter reading initialized
{
	// Select Vref=AVcc
	ADMUX |= (1<<REFS0)|(1<<REFS1);
	//set prescaller to 128 and enable ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	ADCSRA|=(1<<ADIE);// intruppet
	ADMUX = (ADMUX & 0xF0) | (1 & 0x0F);
	ADCSRA |= (1<<ADSC);
}

ISR(ADC_vect){
	
	tempNum=ADC;
	if (anlogSelect)
	{
		//Battry
		batteryLvel=tempNum;
		if (batteryLvel<820)
		{	eeprom_write_float((float*)0,meterReading);
			PORTB&=~(1<<0);
			_delay_ms(1000);
		}
		
		
		ADMUX = (ADMUX & 0xF0) | (0 & 0x0F);
	} 
	else
	{//Current
		currentLevel=tempNum;
		TimeCounter++;
		if (TimeCounter>48)
		{TimeCounter=0;	
		
		maxVal=0;
		for(uint8_t i=0;i<48;i++){
		if (maxVal<tempset[i])
		{maxVal=tempset[i];
		}
		}
		recentMax=maxVal;
		meterReading+=maxVal/180000000;
		/////////////////////////////////////
		
		
		}
		tempset[TimeCounter]=currentLevel;
		
		ADMUX = (ADMUX & 0xF0) | (1 & 0x0F);
	}
	anlogSelect=!anlogSelect;
	ADCSRA |= (1<<ADSC);
	
	/*(1/8000000)*128=1.6*10(^-5)*/
	/*1s/(1.6*10(^-5)*13)=4808*/
	
	
}

bool checkGSM(){
	for(uint8_t i=0;i<20;i++){
		USART_TxStringln("AT");
		if (USART_ReciveWait("OK",1000))
		{
			USART_ClearRX();
			break;
		}
		if (i==19)
		{return 0;
		}
	}
	USART_TxStringln("AT+CPIN?");
	if (USART_ReciveWait("READY",10000)==1)                 //Command and time out
	{
		USART_ClearRX();
	//	return 1;
	}
	else
	{
		USART_ClearRX();
		return 0;
	}
	
	for(uint8_t i=0;i<20;i++){
	USART_TxStringln("AT+CSQ");
		_delay_ms(500);
		strncpy(lengthcal,strstr(rxvalue,"CSQ: ")+5,2);
		gsm=atoi(lengthcal);
		if (gsm>15)
		{break;
		}
		else{
		USART_ClearRX();
		_delay_ms(1000);		
		}
	USART_ClearRX();	
	}
	
	
	
	
	
	
	
	
}

bool setupGSM(){
	//******SMS Functions
	USART_TxStringln("AT+CMGF=1");
	USART_ReciveWait("OK",1000,1);
	_delay_ms(200);
	USART_ClearRX();
	
	USART_TxStringln("AT+CNMI=1,2,0,0,0");
	USART_ReciveWait("OK",1000,1);
	_delay_ms(200);
	USART_TxStringln("AT+DDET=1,0,0");
	USART_ReciveWait("OK",1000,1);
	_delay_ms(200);
	USART_ClearRX();
	
	//****Data function
	
	USART_TxStringln("AT+SAPBR=3,1,Contype,GPRS");
	USART_ReciveWait("OK",1000,1);
	_delay_ms(200);
	
	sprintf(lengthcal,"AT+SAPBR=3,1,APN,%s",apn);
	USART_TxStringln(lengthcal);
	USART_ReciveWait("OK",1000);
	_delay_ms(200);
	
	
}


bool uploaddataPOST(){
	bool returnValue=0;
	USART_TxStringln("AT+SAPBR =1,1");
	_delay_ms(200);
	USART_ClearRX();
	
	
	USART_TxStringln("AT+SAPBR=2,1");
	USART_ReciveWait("OK",3000);
	_delay_ms(100);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPINIT");
	_delay_ms(300);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPSSL=1");
	_delay_ms(200);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPPARA=CID,1");
	USART_ReciveWait("OK",1000);
	_delay_ms(100);
	USART_ClearRX();
	
	
	sprintf(reqvest,"AT+HTTPPARA=URL,%s",postserver);
	USART_TxStringln(reqvest);
	_delay_ms(100);
	USART_TxStringln("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
	USART_ReciveWait("OK",1000);
	_delay_ms(100);
	USART_ClearRX();
	
	
	sprintf(reqvest,"unitw=%f&wattw=%f",30.5,2.1);
	
	sprintf(lengthcal,"AT+HTTPDATA=%d,10000",strlen(reqvest));
	USART_TxStringln(lengthcal);
	USART_ReciveWait("OK",1000);
	_delay_ms(100);
	USART_ClearRX();
	
	USART_TxStringln(reqvest);
	USART_ReciveWait("OK",10000);
	_delay_ms(100);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPACTION=1");
	USART_ReciveWait("+HTTPACTION:",10000);
	_delay_ms(100);
	USART_ClearRX();
	

	
	USART_TxStringln("AT+HTTPREAD");
	if (USART_ReciveWait("Off",500))
	{PORTB|=(1<<0);
		eeprom_write_byte((uint8_t*)5,1);
	}
	else if (USART_ReciveWait("On",500))
	{PORTB&=~(1<<0);
		eeprom_write_byte((uint8_t*)5,0);
	}
	
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPTERM");
	USART_ReciveWait("OK",1000);
	_delay_ms(100);
	USART_ClearRX();
	return returnValue;
	
}
