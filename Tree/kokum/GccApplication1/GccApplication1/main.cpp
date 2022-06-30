#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include "USART.h"
#include "SoftUART.h"
#include "LCDI2C.h"

//define webdata
char*wifiusername="\"SLT-HOME-WIFI\"";
char*wifipassword="\"danusha1234\"";

char*server="api.thingspeak.com";

char*postreqvest="POST /tree/write.php HTTP/1.0\r\n";

char tempchar[100]={0};
char tempnum[10]={0};	

void ESP_Start();
void ESP_sendGET(uint16_t hum);
void ESP_sendPOST(uint8_t hum,float tempurature, float lux);	
void ESP_ReadWeb();
void InitADC();
uint16_t ReadADC(uint8_t ADCchannel);

int main(){
	SoftUART_Init();
	USART_Init(9600);
	LcdInit(0x27);
	InitADC();
		
	ESP_Start();
	
	
	while(1){
	uint16_t adc=ReadADC(0)/10.24;
	SoftUSART_TxNumber(adc);
	SoftUSART_tx_strln("");
	ESP_sendGET(adc);
	_delay_ms(5000);
		
	}
	
	
}

void ESP_Start(){
	///*
	USART_TxString("AT+RST\r\n");
	if (USART_ReciveWait("OK",4000)==1)                 //Command and time out
	{SoftUSART_tx_strln("ESP Reset");
	LcdSetCursor(0,4,"ESP Reset");
	_delay_ms(3000);
	} 
	else
	{SoftUSART_tx_strln("ESP not found");
	LcdSetCursor(0,0,"ESP not Found");
		while(1);
	}
	///////////////////////////////////////////////
	USART_TxString("at\r\n");
	USART_ReciveWait("OK");
	
	/////////////////////////////////////////////
	
	
	USART_TxStringln("AT+CWMODE=1");
	if (USART_ReciveWait("OK")==1)
	{SoftUSART_tx_strln("Mode Selected");
		LcdCommand(LCD_CLEARDISPLAY);
		LcdSetCursor(0,3,"Connecting");
		LcdSetCursor(1,3,"To Router");
		LcdCommand(LCD_DISPLAYON|LCD_BLINKON|LCD_DISPLAYCONTROL);
		}
	else
	{SoftUSART_tx_strln("Check ESP");
		LcdCommand(LCD_CLEARDISPLAY);
		LcdSetCursor(0,0,"Check ESP");
		while(1);
	}
	/////////////////////////////////////////////
	USART_TxStringln("AT+CIPMUX=1");
	USART_ReciveWait("OK");
	////////////////////////
	memset(tempchar,0,100);
	strcat(tempchar,"AT+CWJAP=");
	strcat(tempchar,wifiusername);
	strcat(tempchar,",");
	strcat(tempchar,wifipassword);
	USART_TxStringln(tempchar);
	if (USART_ReciveWait("GOT",10000)==1)
	{SoftUSART_tx_strln("Successfully Connected to router");
		LcdCommand(LCD_CLEARDISPLAY);
		LcdCommand(LCD_DISPLAYON|LCD_BLINKOFF|LCD_DISPLAYCONTROL);
		LcdSetCursor(0,3,"Connected");
		LcdSetCursor(1,3,"to router");
	}
	else
	{SoftUSART_tx_strln("Check router");
		LcdCommand(LCD_CLEARDISPLAY);
		LcdSetCursor(0,2,"Check Router");
		while(1);
	}
	_delay_ms(2000);
	////////////////////////////////

	
}


void ESP_sendGET(uint16_t hum){
	
	memset(tempchar,0,100);
	strcat(tempchar,"AT+CIPSTART=4,\"TCP\",\"");
	strcat(tempchar,server);
	strcat(tempchar,"\",80");
	USART_TxStringln(tempchar);                             //start
	USART_ReciveWait("OK");
	_delay_ms(2000);
	
	memset(tempchar,0,100);
	strcat(tempchar,"GET https://api.thingspeak.com/update?api_key=0Q1HAU32Y2P8JO15&field1=");
	itoa(hum,tempnum,10);
	strcat(tempchar,tempnum);
	strcat(tempchar,"\r\n");
	
	USART_TxString("AT+CIPSEND=4,");
	itoa(strlen(tempchar),tempnum,10);
	USART_TxString(tempnum);
	USART_TxString("\r\n");
	USART_ReciveWait(">");
	
	USART_TxString(tempchar);
	USART_TxString("\r\n");
	USART_ReciveWait("CLOSED",10000);
}

void ESP_sendPOST(uint8_t hum,float tempurature, float lux){
	
	memset(tempchar,0,100);
	strcat(tempchar,"AT+CIPSTART=4,\"TCP\",\"");
	strcat(tempchar,server);
	strcat(tempchar,"\",80");
	USART_TxStringln(tempchar);                             //start
	USART_ReciveWait("OK");
	
	
	memset(tempchar,0,100);
	strcat(tempchar,"up=H");  //calculate update string command size
	itoa(hum,tempnum,10);
	strcat(tempchar,tempnum);
	strcat(tempchar,"T");
	itoa(tempurature,tempnum,10);
	strcat(tempchar,tempnum);  //tempurature
	strcat(tempchar,"L");
	dtostrf(lux,4,3,tempnum);
	strcat(tempchar,tempnum);//lux
	strcat(tempchar,"E\r\n");
	
	
	
	char posttempchar[300]={0};        //calculate data string size
	strcat(posttempchar,postreqvest); 
	strcat(posttempchar,"Host: ");
	strcat(posttempchar,server);
	strcat(posttempchar,"\r\n");
	strcat(posttempchar,"Accept: */*\r\n");	
	strcat(posttempchar,"Content-Length: ");
	itoa(strlen(tempchar),tempnum,10);
	strcat(posttempchar,tempnum);
	strcat(posttempchar,"\r\n");
	strcat(posttempchar,"Content-Type: application/x-www-form-urlencoded\r\n");
	strcat(posttempchar,"\r\n");
	strcat(posttempchar,tempchar);
	strcat(posttempchar,"\r\n");		
	
	USART_TxString("AT+CIPSEND=4,");
	itoa(strlen(posttempchar),tempnum,10);
	USART_TxString(tempnum);
	USART_TxString("\r\n");
	USART_ReciveWait(">");
	
	USART_TxString(posttempchar);
	
	if(USART_ReciveWait("CLOSED",4000)==1){
	SoftUSART_tx_str("Data added Successfully.\r\n");	
	}
	else{
	SoftUSART_tx_str("Update failed.\r\n");	
	}
	
}

void ESP_ReadWeb(){
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,2,"Reading Data");
	LcdSetCursor(1,0,server);
	LcdCommand(LCD_CURSORMOVE|LCD_MOVERIGHT|LCD_CURSORSHIFT);
	LcdCommand(LCD_DISPLAYON|LCD_BLINKON|LCD_DISPLAYCONTROL);
	
	memset(tempchar,0,100);
	strcat(tempchar,"AT+CIPSTART=4,\"TCP\",\"");
	strcat(tempchar,server);
	strcat(tempchar,"\",80");
	USART_TxStringln(tempchar);                             //start
	USART_ReciveWait("OK");
	
	memset(tempchar,0,100);
	strcat(tempchar,"GET /tree/readwrite.php\r\n");
	
	USART_TxString("AT+CIPSEND=4,");
	itoa(strlen(tempchar),tempnum,10);
	USART_TxString(tempnum);
	USART_TxString("\r\n");
	USART_ReciveWait(">");
	
	
	USART_TxString(tempchar);
	USART_ReciveWait("CLOSED",4000,1);
	char*webRecive=strrchr(rxvalue,'*');
	SoftUSART_tx_str(webRecive);
	char*webhum;
	char*webtemp;
	char*weblux;
	LcdCommand(LCD_CLEARDISPLAY);
	LcdCommand(LCD_DISPLAYON|LCD_BLINKOFF|LCD_DISPLAYCONTROL);
	LcdSetCursor(0,0,"Hum  Tem  LUX");
	
	USART_clear();
	
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