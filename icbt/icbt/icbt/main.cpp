/*
 * icbt.cpp
 *
 * Created: 4/9/2021 10:00:16 PM
 * Author : Dilan
 */ 


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <setjmp.h>
#include <stdio.h>
#include "oled_ssd1306.h"
#include "USART.h"
//#include "SoftUART.h"

char*tempcharacter;
char getreqvest[80];
char lengthcal[20];
char*server="192.168.1.102";



uint16_t reading;
uint8_t xside=0;
uint8_t saveReading[128];

jmp_buf mainstart;





//************************************************************************
bool mainFunctionInit();
bool checkEsp();
bool setupESP();
bool checkRouter();
bool routerconnect();
uint16_t ReadADC(uint8_t ADCchannel);
void InitADC();
void pulseRead();
 bool uploaddata();
//************************************************************************


int main(void)
{	setjmp (mainstart);
	mainFunctionInit();
	if (!(PINB&0b00000001)){
		
		routerconnect();
	}
	if (!checkEsp())
	{	
		oled_clear_screen();
		oled_gotoxy(0,2);
		oled_font_size(0);
		oled_write("ESP not connected");
		oled_gotoxy(0,3);
		oled_write("Device Restarting. . .");
		_delay_ms(500);
		longjmp (mainstart, 1);
	}
	else{
	oled_clear_screen();
	oled_gotoxy(0,2);
	oled_font_size(0);
	oled_write("ESP OK");
	}
	//****************************
	if (!setupESP())
	{longjmp (mainstart, 1);
	}
	else{
	oled_clear_screen();
	oled_gotoxy(0,3);
	oled_font_size(0);
	oled_write("Setup ESP");	
		
	}
	//***************************
	if (!checkRouter())
	{	
		oled_gotoxy(0,4);
		oled_font_size(0);
		oled_write("Router Fail");
		_delay_ms(500);
		longjmp (mainstart, 1);
		
	}
	else{
	oled_gotoxy(0,4);
	oled_font_size(0);
	oled_write("Router Connected");	
	_delay_ms(500);
	}
	
	
	oled_clear_screen();
	
	while(1){
		pulseRead();
		_delay_ms(50);
		
		
		
	}
	
	
}

void InitADC()  //meter reading initialized
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

bool mainFunctionInit(){
	_delay_ms(500);
	oled_init();
	USART_Init(9600);
	PORTB|=(1<<0);// switch pull up
	//SoftUART_Init();
	//SoftUSART_tx_strln("Starterd");
	InitADC();
	oled_gotoxy(0,2);
	oled_font_size(1);
	oled_write("Welcome");
	oled_gotoxy(5,5);
	oled_font_size(0);
	oled_write("P Tec");
	_delay_ms(3000);
	
}

bool checkEsp(){
	USART_ClearRX();
	USART_TxStringln("AT");
	_delay_ms(100);
	USART_TxStringln("AT");
	_delay_ms(100);
	if (USART_ReciveWait("OK",200))
	{USART_ClearRX();
		return 1;
		
	}
	else{
		return 0;
	}
}

bool setupESP(){
	USART_ClearRX();
	USART_TxStringln("AT+CWMODE=1");
	_delay_ms(100);
	if (USART_ReciveWait("OK",500))
	{USART_ClearRX();
	} 
	else
	{	
		return 0;
	}
	
	
	USART_TxStringln("AT+CIPMUX=0");
	_delay_ms(100);
	if (USART_ReciveWait("OK",500))
	{
		USART_ClearRX();
		
	}
	else
	{	USART_TxStringln("AT+RST");
		_delay_ms(1000);
		
		return 0;
	}
	
	
	
	
	return 1;
	
}

bool checkRouter(){
	USART_TxStringln("AT+CWJAP?");
	if (USART_ReciveWait("No AP",1000))
	{
		return 0;
	} 
	else
	{
		return 1;
	}
	
}

bool routerconnect(){
	oled_clear_screen();
	oled_gotoxy(0,2);
	oled_font_size(0);
	oled_write("Push WPS button");
	//**********************
	USART_TxStringln("AT+CWMODE=1");
	_delay_ms(100);
	if (USART_ReciveWait("OK",500))
	{USART_ClearRX();
	}
	else
	{
		return 0;
	}
	//**********************
	USART_TxStringln("AT+WPS=1");
	if (USART_ReciveWait("OK",500))
	{USART_ClearRX();
	}
	else
	{
		return 0;
	}
	
	for(uint8_t i=0;i<128;i++){
	oled_write_dot(i,31);	
	if (USART_ReciveWait("success",500))
	{
		_delay_ms(2000);
		USART_TxStringln("AT+WPS=0");
		_delay_ms(500);
		oled_gotoxy(0,4);
		oled_font_size(0);
		oled_write("Router Connected");
		_delay_ms(1000);
		return 1;
	}
	if (i==127)
	{
		USART_TxStringln("AT+WPS=0");
		_delay_ms(500);
		oled_gotoxy(0,4);
		oled_font_size(0);
		oled_write("Router Faild");
		_delay_ms(1000);
		return 0;
	}
	}
	
 }
 
 void pulseRead(){
	 
	 if (PINC&0b00000110)
	 {
		 
	 } 
	 else
	 {	reading=ReadADC(0);
		oled_gotoxy(0,0);
		oled_write("      ");
		oled_gotoxy(0,0);
		char lcd[6];
		itoa((reading-512),lcd,10);
		oled_write_str(lcd); 
		
		xside++;
		if (xside==1)
		{oled_gotoxy(10,0);
		oled_write("      ");
		oled_gotoxy(10,0);
		char lcd[6];
		uint16_t BatRead=ReadADC(3);
		
		if (BatRead>450)
		{sprintf(lcd,"%dV",BatRead/45);
			oled_write_str(lcd);
		} 
		else
		{itoa((BatRead-345)*2.13,lcd,10);

			oled_write_str(lcd);
		}
		
		
		
		
		

		
		}
		if (xside>127)
		{xside=0;
		uploaddata();
		oled_clear_screen();
		}
		
		saveReading[xside]=reading/4;
		uint8_t cal=(reading*(-0.0469))+63;
		oled_write_dot(xside,cal);
	 }
	 
 }
 
 bool uploaddata(){
	 
	 USART_ClearRX();
	 sprintf(getreqvest,"AT+CIPSTART=\"TCP\",\"%s\",80",server);
	 USART_TxStringln(getreqvest);
	 if (USART_ReciveWait("CONNECT",3000))
	 {USART_ClearRX();
	 }
	 else
	 {
		 USART_ClearRX();
		 return 0;
	 }
	 
	 
	
	 
	 USART_TxStringln("AT+CIPSEND=538");
	 if (USART_ReciveWait("OK",2000))
	 {
		 USART_ClearRX();
		 _delay_ms(100);
	 }
	 else
	 {
		 USART_ClearRX();
		 return 0;
	 }
	 USART_TxString("GET /icbt/write.php?up=");
	 USART_TxString("S");
	 for(uint8_t i=0;i<128;i++){
		
		 sprintf(lengthcal,"D%03d",saveReading[i]);
		 USART_TxString(lengthcal);
		 
	 }
		USART_TxStringln("");
	 
	 
	 if (USART_ReciveWait("DATA",2000))
	 {
		 USART_ClearRX();
		 ;
		 return 1;
	 }
	 else
	 {
		 
		 
		 USART_ClearRX();
		 return 0;
	 }

	 
 }