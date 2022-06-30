/*
 * both.cpp
 *
 * Created: 4/25/2021 11:28:16 AM
 * Author : Dilan
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <setjmp.h>
#include <stdio.h>
#include "USART.h"
//#include "SoftUART.h"

char*tempcharacter;
char reqvest[120];
char lengthcal[40];
char*postserver="http://powerpanel123.000webhostapp.com/writepost.php";
char*getserver="http://powerpanel123.000webhostapp.com/write.php";
char*apn="hutch3g";

uint8_t numdata=0;
float vol;
float amp;

jmp_buf mainstart;





//************************************************************************
void mainFunctionInit();
bool checkGSM();
bool setupGSM();
uint16_t ReadADC(uint8_t ADCchannel);
void InitADC();
bool uploaddataGET();
bool uploaddataPOST();
//************************************************************************

int main(void)
{	setjmp (mainstart);
	_delay_ms(500);
	mainFunctionInit();
	
	if (!checkGSM())
	{longjmp(mainstart,1);
	}
	setupGSM();
	
    
	
    while (1) 
    {
		numdata++;
		vol+=0.3;
		amp+=0.2;
		if(numdata==8){
			numdata=0;
		}
		if(vol>300){
			vol=0;
		}
		if(amp>50){
			amp=0;
		}
		//uploaddataGET();
		uploaddataPOST();
    }
}

void mainFunctionInit(){
	USART_Init(9600);
	USART_ClearRX();
	
}

bool checkGSM(){
	for(uint8_t i=0;i<20;i++){
		USART_TxStringln("AT");
		if (USART_ReciveWait("OK",1000))
		{USART_ClearRX();
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
		return 1;
	}
	else
	{
		return 0;
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

bool uploaddataGET(){
	bool returnValue=0;
	USART_TxStringln("AT+SAPBR =1,1");
	_delay_ms(200);
	USART_ClearRX();
	
	USART_TxStringln("AT+SAPBR=2,1");
	USART_ReciveWait("OK",3000);
	_delay_ms(100);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPINIT");
	USART_ReciveWait("OK",1000);
	_delay_ms(100);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPPARA=CID,1");
	USART_ReciveWait("OK",1000);
	_delay_ms(100);
	USART_ClearRX();
	
	//https://powerpanel123.000webhostapp.com/write.php?up=L101V34.2A565E
	sprintf(reqvest,"AT+HTTPPARA=URL,%s?up=L%03uV%3.1fA%3.1fE",getserver,numdata,vol,amp);
	USART_TxStringln(reqvest);
	USART_ReciveWait("OK",1000);
	_delay_ms(100);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPACTION=0");
	USART_ReciveWait("+HTTPACTION:",10000);
	_delay_ms(100);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPREAD");
	if (USART_ReciveWait("ADDED",5000))
	{returnValue=1;
	}
	_delay_ms(100);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPTERM");
	USART_ReciveWait("OK",1000);
	_delay_ms(100);
	USART_ClearRX();
	 return returnValue;
	
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
	
	USART_TxStringln("AT+HTTPSSL=0");
	_delay_ms(200);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPPARA=CID,1");
	USART_ReciveWait("OK",1000);
	_delay_ms(100);
	USART_ClearRX();
	
	//https://powerpanel123.000webhostapp.com/write.php?up=L101V34.2A565E
	sprintf(reqvest,"AT+HTTPPARA=URL,%s",postserver);
	USART_TxStringln(reqvest);
	_delay_ms(100);
	USART_TxStringln("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
	USART_ReciveWait("OK",1000);
	_delay_ms(100);
	USART_ClearRX();
	
	//up=L000V34.2A585E
	sprintf(reqvest,"up=L%03uV%3.1fA%3.1fE",numdata,vol,amp);
	
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
	if (USART_ReciveWait("ADDED",100))
	{returnValue=1;
	}
	_delay_ms(100);
	USART_ClearRX();
	
	USART_TxStringln("AT+HTTPTERM");
	USART_ReciveWait("OK",1000);
	_delay_ms(100);
	USART_ClearRX();
	return returnValue;
	
}