/*
 * emmanuel.cpp
 *
 * Created: 12/31/2021 12:54:50 PM
 * Author : dilan
 */ 
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
char*postserver="http://gsmnet.emmanuelslanka.com/read.php";

bool uploaddataPOST(bool getbottle,bool hasbottle,bool checks);
bool readdataPOST();
void stepper(bool dir,uint8_t channel, uint16_t degree);	

#define machineNum 1
bool checkS=0;
bool getbottle=0;

#define stepperddr DDRC
#define stepperport PORTC
uint16_t stepperdiv[6]={8,8,8,8,8,8};
#define stepdirddr DDRB
#define stepdirport PORTB
#define stepdir 2


#define solddr DDRD
#define solport PORTD
#define solout 5
#define solin 6
#define spkr 7

int main(void)
{	
	DDRC=0b111111;//stepper
	stepdirddr|=(1<<stepdir);
	solddr|=(1<<solout)|(1<<solin)|(1<<spkr);
	stepper(1,0,360);
	
	
    setjmp (mainstart);
    _delay_ms(500);
    sei();
	mainFunctionInit();
	if (!checkGSM())
	{longjmp(mainstart,1);
	}
	setupGSM();

	solport|=(1<<spkr);
	_delay_ms(50);
	solport&=~(1<<spkr);
	_delay_ms(50);
	solport|=(1<<spkr);
	_delay_ms(50);
	solport&=~(1<<spkr);
	
	
	
    while (1) 
    {
		
		if (!readdataPOST())
		{longjmp(mainstart,1);
		}
		
		if (checkS)
		{
			solport|=(1<<solin);
			uint32_t  cycounter=0;
			bool bottle=0;
			while(1){
				if (PINB&(1<<0))
				{	solport|=(1<<spkr);
					_delay_ms(50);
					solport&=~(1<<spkr);
					bottle=1;
					break;
				}
				
				
				cycounter++;
				if (cycounter>2000)
				{break;
				}
				_delay_ms(10);
			}
			
			if (bottle)
			{uploaddataPOST(0,1,0);//has bottle 1
			solport|=(1<<spkr);
			_delay_ms(50);
			solport&=~(1<<spkr);
			_delay_ms(50);
			solport|=(1<<spkr);
			_delay_ms(50);
			solport&=~(1<<spkr);
			_delay_ms(10000);//button tome
			uploaddataPOST(0,0,0);//has bottle 1
			solport&=~(1<<solin);	
			}
			
			else{
			uploaddataPOST(0,0,0);//has bottle 0	
			solport&=~(1<<solin);
			}
			
			
			solport&=~(1<<solin);
			
			checkS=0;
		}//end checks
		
		
		if (getbottle)
		{	solport|=(1<<spkr);
			_delay_ms(50);
			solport&=~(1<<spkr);
			solport|=(1<<solout);
			_delay_ms(2000);
			solport&=~(1<<solout);
			uploaddataPOST(0,0,0);//get bottle 0
			getbottle=0;
		}
		
					
		
		
		_delay_ms(200);
    }
}


void mainFunctionInit(){
	USART_Init(9600);
	USART_ClearRX();
	DDRD|=(1<<2);// rst
	
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

bool uploaddataPOST(bool getbottle,bool hasbottle,bool checks){
	
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
	
	
	sprintf(reqvest,"man=%d&gtb=%u&hb=%u&chks=%u",machineNum,getbottle,hasbottle,checks);
	
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
	if (USART_ReciveWait("ctdgnhn",5000))
	{returnValue=1;
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

bool readdataPOST(){
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
	

	sprintf(reqvest,"rd=%d",machineNum);
	
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
	
	memset(lengthcal,0,40);
	
	USART_TxStringln("AT+HTTPREAD");
	if (USART_ReciveWait("ctd",5000))
	{
	strncpy(lengthcal,strstr(rxvalue,"ctdchs")+6,1);
	checkS=atoi(lengthcal);
	memset(lengthcal,0,40);
	strncpy(lengthcal,strstr(rxvalue,"ctdchs")+9,1);
	getbottle=atoi(lengthcal);
	_delay_ms(25);
	USART_ClearRX();
	returnValue =1;	
	} 
	
	else{
	USART_ClearRX();
	return 0;	
	}
	
	
	
	
	
	
	/*
	USART_TxStringln("AT+HTTPTERM");
	USART_ReciveWait("OK",1000);
	_delay_ms(25);
	USART_ClearRX();*/
	return returnValue;	
	
	
}

void stepper(bool dir,uint8_t channel, uint16_t degree){
	
	if (dir)
	{stepdirport|=(1<<stepdir);
	} 
	else
	{stepdirport&=~(1<<stepdir);
	}
	
	
	uint32_t limit=(200*(stepperdiv[channel]));
	limit=limit*degree;
	limit=limit/360;
	
	for(uint32_t i=0;i<limit;i++){
		stepperport|=(1<<channel);
		_delay_us(100);
		stepperport&=~(1<<channel);
		_delay_us(100);
	}
	
	
}