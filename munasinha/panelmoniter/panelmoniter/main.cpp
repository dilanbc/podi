/*
 * panelmoniter.cpp
 *
 * Created: 2/12/2021 7:17:56 AM
 * Author : Dilan
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <setjmp.h>
#include <util/delay.h>
#include <stdio.h>
#include "USART.h"
//#include "SoftUART.h"

/*define wifi parameters*/
char*wifiusername="LAHIRU 4G";//SLT-HOME-WIFI
char*wifipassword="lahiru16728";
char *tempcharacter;
char getreqvest[80];
char lengthcal[20];

char*server="192.168.1.102";

char*postreqvest="POST /tree/write.php HTTP/1.0\r\n";

jmp_buf mainstart;
uint8_t i;
bool relay[3];
uint16_t voltage=200;
float amp=2;

/*???????? ???????*/
float sensorValue1 = 0;
float sensorValue2 = 0;
int crosscount = 0;
int climb_flag = 0;
int val[100];
int max_v = 0;
float VmaxD = 0;
float VeffD = 0;
float Veff = 0;

/*current sensor*/
#define calibration_const 355.55
int max_val;
int new_val;
int old_val = 0;
float rms;
float IRMS;

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
	
	
	USART_TxStringln("AT+CIPMUX=1");
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
	
	/*USART_TxStringln("AT+CWQAP");
	while(1);*/
	USART_TxStringln("AT+CWJAP?");
	if (USART_ReciveWait("OK",1000))
	{	_delay_ms(100);
		
		tempcharacter=strstr(rxvalue,wifiusername);
		
		
		if (tempcharacter) //connected no problem
		{return 1;
		} 
		 else // check not connocted or no ap
		{
			tempcharacter=strstr(rxvalue,"No");
			
			if(tempcharacter){
				/*/connect/ */
				
				sprintf(tempcharacter,"AT+CWJAP=\"%s\",\"%s\"",wifiusername,wifipassword);
				
				USART_TxStringln(tempcharacter);
				
				if (USART_ReciveWait("GOT",8000))
				{	
					
					USART_ClearRX();
					return 1;
				} 
				else
				{	USART_ReciveWait("FAIL",5000);
					_delay_ms(2000);
					USART_ClearRX();
					return 0;
				}
			/*end of connect	*/
			}
			else{
				/*/disconnect and connect/*/
				USART_ClearRX();
				USART_TxStringln("AT+CWQAP");
				if (USART_ReciveWait("OK",1000))
				{
					USART_ClearRX();
					
				sprintf(tempcharacter,"AT+CWJAP=\"%s\",\"%s\"",wifiusername,wifipassword);
				
				USART_TxStringln(tempcharacter);
				
				if (USART_ReciveWait("GOT",5000))
				{	
					
					USART_ClearRX();
					return 1;
				} 
				else
				{	USART_ReciveWait("FAIL",5000);
					_delay_ms(2000);
					USART_ClearRX();
					return 0;
				}
					
				} 
								
			/*	/end of disconnect and connect/*/
			}
		}
		
	} //recevewait
	else
	{ return 0;
	}
	
	
}

bool uploaddata(){
	
		
	USART_ClearRX();
	sprintf(tempcharacter,"AT+CIPSTART=4,\"TCP\",\"%s\",80",server);
	USART_TxStringln(tempcharacter);
	if (USART_ReciveWait("CONNECT",3000))
	{USART_ClearRX();
	} 
	else
	{
		USART_ClearRX();
		return 0;
	}
	
	sprintf(getreqvest,"GET /powerpanel/write.php?up=L%d%d%dV%dA%dE\r\n",relay[0],relay[1],relay[2],voltage,amp);
	
	
	sprintf(lengthcal,"AT+CIPSEND=4,%d\r\n",strlen(getreqvest));
	;
	
	USART_TxString(lengthcal);
	if (USART_ReciveWait("OK",2000))
	{
		USART_ClearRX();
	} 
	else
	{
		USART_ClearRX();
		return 0; 
	}
	
	

	
	USART_TxString(getreqvest);
	if (USART_ReciveWait("DATA"))
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
float ReadVoltage(){
	for ( int i = 0; i < 100; i++ ) {
		sensorValue1 = ReadADC(2);
		if (ReadADC(2) > 511) {
			val[i] = sensorValue1;
		}
		else {
			val[i] = 0;
		}
		_delay_ms(1);
	}

	max_v = 0;

	for ( int i = 0; i < 100; i++ )
	{
		if ( val[i] > max_v )
		{
			max_v = val[i];
		}
		val[i] = 0;
	}
	if (max_v != 0) {


		VmaxD = max_v;
		VeffD = VmaxD / sqrt(2);
		Veff = (((VeffD - 420.76) / -90.24) * -210.2) + 210.2;
	}
	else {
		Veff = 0;
	}
	
	VmaxD = 0;
	if (Veff<110)
	{return 0;
	}
	return Veff;
}

float readcurrent(){
	new_val = ReadADC(3);
  if(new_val > old_val) {
    old_val = new_val;
  }
  
  else {
   _delay_us(50);
    new_val = ReadADC(3);
    if(new_val < old_val) {
      max_val = old_val;
      old_val = 0;
    }
    
    rms = max_val * 5.00 * 0.707 / 1024;
    IRMS = rms * calibration_const;
	return IRMS;
}
}
void InitADC()
{
	// Select Vref=AVcc
	ADMUX |= (1<<REFS0);
	//set prescaller to 128 and enable ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	
}



int main(void)
{	if (setjmp (mainstart))
{ 
}
	InitADC();
	DDRB&=~(1<<6); //pin 9
	DDRB&=~(1<<7); //pin 10
	DDRD&=~(1<<5); //pin 11
	
    USART_Init(9600);
	
	
	
	if (!checkEsp())
	{	_delay_ms(1000);
		longjmp (mainstart, 1);
	} 
	
	if (!setupESP())
	{	_delay_ms(1000);
		longjmp (mainstart, 1);
	}
	
	
	_delay_ms(100);
    while (1) 
    {	relay[0]=!((PINB&0b1000000)>>6);
		relay[1]=!((PINB&0b10000000)>>7);
		relay[2]=!((PIND&0b100000)>>5);
		voltage=ReadVoltage();
		readcurrent();
		if (!uploaddata())
		{	_delay_ms(5000);
			longjmp (mainstart, 1);
		}
		
    }
}

