/*
 * kurunagala.cpp
 *
 * Created: 11/4/2021 9:00:33 PM
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
#include "stdio.h"
#include "USART.h"

jmp_buf mainstart;

void mainFunctionInit();
bool checkGSM();
void setupGSM();
void setupTimer();
void sendSMS(uint8_t sender,char*msg);


char lengthcal[40];//string memory
int gsm;//gsm signal calculation
char numbr[15]={};
char filtere[9];
	
char adminNum[9]={0};
char costom1[9]={0};
char costom2[9]={0};
	
int8_t temp=0;
bool lockState=0;

int main(void)
{
   setjmp (mainstart);
   _delay_ms(500);
   setupTimer();
   sei();
   DDRB|=(1<<0);
   
   //eeprom_write_block((char*)"779394345",(uint8_t*)0,9);
  // eeprom_write_block((char*)"740083133",(uint8_t*)10,9);
   //eeprom_write_block((char*)"740083133",(uint8_t*)20,9);
   
   eeprom_read_block(adminNum,(uint8_t*)0,9);
   eeprom_read_block(costom1,(uint8_t*)10,9);
   eeprom_read_block(costom2,(uint8_t*)20,9);
   if (eeprom_read_byte((uint8_t*)50))
   { lockState=1;

   }
   else
   {lockState=0;
   }
   
   mainFunctionInit();
	if (!checkGSM())
	{longjmp(mainstart,1);
	}
	setupGSM();
	
	
	USART_ClearRX();
	OCR1A=3125;
	sendSMS(0,"The GSM Unit was Powered by Electricity.\n (SMN POWER ELECTRICALS\n077-9394345)");
	sendSMS(1,"The GSM Unit was Powered by Electricity.\n (SMN POWER ELECTRICALS\n077-9394345)");
	sendSMS(2,"The GSM Unit was Powered by Electricity.\n (SMN POWER ELECTRICALS\n077-9394345)");
	OCR1A=3125*5;
	
    while (1) 
    {
		
		if (USART_ReciveWait("+CMT",200))
		{	_delay_ms(100);
			
			
			strncpy(filtere,strstr(rxvalue,"+CMT")+10,9);
			
			uint8_t checkNum=0;
			
			for(uint8_t i=0;i<9;i++){// check admin num
			if (filtere[i]==adminNum[i])
			{checkNum++;
			} 
			else
			{break;
			}
			}
			
			if (checkNum>=8)// start admin functions
			{OCR1A=3125;
			
			if (USART_ReciveWait("RG",200)){
				strncpy(filtere,strstr(rxvalue,"RG")+4,9);
				eeprom_write_block((char*)filtere,(uint8_t*)0,9);
				sendSMS(0,"Admin Number Changed.");
				 eeprom_read_block(adminNum,(uint8_t*)0,9);
			}
			
			if (USART_ReciveWait("Lock",200)){
				
				eeprom_write_byte((uint8_t*)50,1);
				sendSMS(0,"The System is Locked by the Admin.");
				//sendSMS(1,"The system is Locked by the admin.");
				//sendSMS(2,"The system is Locked by the admin.");
				lockState=1;
				
			}
			if (USART_ReciveWait("Active",200)){
				eeprom_write_byte((uint8_t*)50,0);
				sendSMS(0,"The System is Unlocked by the Admin.");
				sendSMS(1,"The System is Unlocked by the Admin.");
				sendSMS(2,"The System is Unlocked by the Admin.");
				lockState=0;
			}
			
			else if (USART_ReciveWait("SMN1",200)){
				strncpy(filtere,strstr(rxvalue,"SMN1")+6,9);
				eeprom_write_block((char*)filtere,(uint8_t*)10,9);
				sendSMS(1,"Your Number is registered as Operator.\n (SMN POWER ELECTRICALS\n077-9394345)");
				sendSMS(0,"Operator 1 successfully registered.");
				 eeprom_read_block(costom1,(uint8_t*)10,9);
			}
			
			
			
			
			else if (USART_ReciveWait("SMN2",200)){
				strncpy(filtere,strstr(rxvalue,"SMN2")+6,9);
				eeprom_write_block((char*)filtere,(uint8_t*)20,9);
				sendSMS(2,"Your Number is registered as Operator.\n (SMN POWER ELECTRICALS\n077-9394345)");
				sendSMS(0,"Operator 2 successfully registered.");				
			 eeprom_read_block(costom2,(uint8_t*)20,9);
				
			}
			
			
			
			else if (USART_ReciveWait("ON",200)){
				if (PINB&(1<<0))
				{	
					sendSMS(0,"Sorry!");
				} 
				else
				{
				PORTB|=(1<<0);
				
				sendSMS(0,"System ON");
				}
				
										
			}
			
			else if (USART_ReciveWait("OFF",200)){
				
				if (PINB&(1<<0))
				{PORTB&=~(1<<0);
					
					sendSMS(0,"System OFF ");
					sendSMS(1,"The System is Deactivated by the Admin");
					sendSMS(2,"The System is Deactivated by the Admin");
					
				}
				else
				{	
					sendSMS(0,"Sorry!");
				}
				
				
										
			}
				
			OCR1A=3125*5;
		}// end admin functions
			
			
			 checkNum =0;
			for(uint8_t i=0;i<9;i++){// check op1 num
				if (filtere[i]==costom1[i])
				{checkNum++;
				}
				else
				{break;
				}
			}
			
			if (checkNum>=8){// start oparator 1 functions
				OCR1A=3125;
				if (lockState)
				{sendSMS(1,"The system is Locked by the Admin.");
				} 
				else
				{
					if (USART_ReciveWait("ON",200)){
						
						if (PINB&(1<<0)){
							
							sendSMS(1,"Sorry!");
							
						}
						else{
							PORTB|=(1<<0);
							sendSMS(1,"System On");
							sendSMS(2,"The System is Activated by the First Operator");
							sendSMS(0,"The System is Activated by the First Operator");
						}
						
						
						
					}
					
					else if (USART_ReciveWait("OFF",200)){
						
						if (PINB&(1<<0))
						{	PORTB&=~(1<<0);
							sendSMS(1,"System OFF");
							sendSMS(2,"The System is Deactivated by the First Operator");
							sendSMS(0,"The System is Deactivated by the First Operator");
						}
						else
						{
							sendSMS(1,"Sorry!");
							
						}
				}
				
				 
					
					
									
					
					
					
				}
				
			OCR1A=3125*8;	
			}// end oparator1
			
			
///////////////////////////////////////////////////////////////////////////			
			 checkNum =0;
			 for(uint8_t i=0;i<9;i++){// check op2 num
				 if (filtere[i]==costom2[i])
				 {checkNum++;
				 }
				 else
				 {break;
				 }
			 }
			 
			 if (checkNum>=8){// start operator 2 functions
			OCR1A=3125;
			
			if (lockState)
			{sendSMS(2,"The system is Locked by the admin.");
			} 
			else
			{if (USART_ReciveWait("ON",200)){
				
				if (PINB&(1<<0)){
					sendSMS(2,"Sorry!");
				}
				else{
					PORTB|=(1<<0);
					sendSMS(2,"System On");
					sendSMS(1,"The System is Activated by the Second Operator");
					sendSMS(0,"The System is Activated by the Second Operator");
					
				}
				
				
				
			}
			
			else if (USART_ReciveWait("OFF",200)){
				
				if (PINB&(1<<0))
				{
					PORTB&=~(1<<0);
					
					sendSMS(2,"System OFF");
					sendSMS(1,"The System is Deactivated by the Second Operator");
					sendSMS(0,"The System is Deactivated by the Second Operator");
					
				}
				else
				{
					
					sendSMS(2,"Sorry!");
					
				}
			}
			
			
				
				
				
				
				
				
			}
			
			OCR1A=3125*8;	 
				 
				 
		 }// end oparator2
			
			
		}//cmt
		
    }//while
}

void mainFunctionInit(){
	USART_Init(9600);
	USART_ClearRX();
	DDRB|=(1<<0);// relay
	DDRD|=(1<<2);// rst
	DDRC|=(1<<1);// led
	
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
	OCR1A=3125*5;
}

void setupTimer(){
	TCCR1B|=(1<<WGM12)|(1<<CS12);
	TIMSK1|=(1<<OCIE1A);
	OCR1A=3125;
	
}
ISR(TIMER1_COMPA_vect){
	PORTC^=(1<<1);
}
void sendSMS(uint8_t sender,char*msg){
	eeprom_read_block((char*)numbr,(uint8_t*)(sender*10),9);
	sprintf(lengthcal,"AT+CMGS=\"+94%s\"",numbr);
	USART_TxStringln(lengthcal);
	_delay_ms(200);
	USART_TxString(msg);
	_delay_ms(200);
	USART_Transmit(26);
	_delay_ms(200);
	USART_ClearRX();
	_delay_ms(30000);
	USART_ClearRX();
	
}
