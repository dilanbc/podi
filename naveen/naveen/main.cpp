/*
 * naveen.cpp
 *
 * Created: 4/20/2021 12:27:06 PM
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
char getreqvest[120];
char lengthcal[80];
char*server="192.168.1.108";

char wifiUsername[40];
char wifiPassword[40];
uint16_t loopCount;


jmp_buf mainstart;





//************************************************************************
bool mainFunctionInit();
bool checkEsp();
bool setupESP();
bool checkRouter();
bool makeHost();
uint16_t ReadADC(uint8_t ADCchannel);
void InitADC();
 bool uploaddata();
//************************************************************************

//DTH****************************
#define DTH_DDR DDRC
#define DTH_Port PORTC
#define DTH_Read PINC
#define DTH_Pin PINC1
bool DTH_PinRead() { return (DTH_Read&(1<<DTH_Pin));}
bool DTH_Init();
void DTR_ReadValue();
float ReadHumidity;
float ReadTemp;
//*****************************

/*ph data*/
float phValue;
float calibration_value = 21.34+8.5;
uint16_t analogAV;
float phValueCal;
void ReadPH();
/*end ph data*/

/*tubidity data*/
float tubidity;
uint16_t analogtubidity;
void ReadTubidity();

uint16_t distance;
void distanceRead();

int main(void)
{	setjmp (mainstart);
	mainFunctionInit();
	if (!(PINB&0b00000001)){
		
		makeHost();
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
		
	DTR_ReadValue();
	sprintf(lengthcal,"Humidity  %.1f%%",ReadHumidity);
	oled_gotoxy(0,2);
	oled_write_str(lengthcal);
	sprintf(lengthcal,"Tempurat. %.1fC",ReadTemp);
	oled_gotoxy(0,3);
	oled_write_str(lengthcal);
	
	ReadPH();
	sprintf(lengthcal,"PH value  %.1f",phValue);
	oled_gotoxy(0,4);
	oled_write_str(lengthcal);
	
	/*distanceRead();
	sprintf(lengthcal,"%u",distance);
	oled_gotoxy(0,5);
	oled_write_str(lengthcal);*/
	
	ReadTubidity();
	sprintf(lengthcal,"Turbidity  %.1f",tubidity);
	oled_gotoxy(0,6);
	oled_write_str(lengthcal);
	
	loopCount++;
	sprintf(lengthcal,"%d",loopCount);
	oled_gotoxy(0,7);
	oled_write_str(lengthcal);
	uploaddata();
	_delay_ms(2000);	
		
		
		
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

bool DTH_Init(){
	DTH_DDR|=(1<<DTH_Pin); //Configure RD0 as output
	DTH_Port&=~(1<<DTH_Pin); //RD0 sends 0 to the sensor
	_delay_ms(18);
	DTH_Port|=(1<<DTH_Pin); //RD0 sends 1 to the sensor
	_delay_us(30);
	DTH_DDR&=~(1<<DTH_Pin); //Configure RD0 as input
	bool	Check_bit = 0;
	_delay_us(40);
	if (DTH_PinRead()==0){
		_delay_us(80);
		if (DTH_PinRead()!=0){
			Check_bit = 1;
		}
	_delay_us(50);}
	return Check_bit;
}

void DTR_ReadValue(){
	uint8_t dthloopCount;
	float TempReadHumidity=ReadHumidity;
	float TempReadTemp=ReadTemp;
	DTH_Init();
	uint8_t read[5];
	for(uint8_t i=0;i<5;i++){
		uint8_t data, for_count;
		for(for_count = 0; for_count < 8; for_count++){
			while(!DTH_PinRead()){
				dthloopCount++;
				if (dthloopCount>100)
				{break;
				}
				}
			_delay_us(30);
			if(DTH_PinRead() == 0){
				data&= ~(1<<(7 - for_count)); //Clear bit (7-b)
			}
			else{
				data|= (1 << (7 - for_count)); //Set bit (7-b)
				dthloopCount=0;
				while(DTH_PinRead()){
					{
						dthloopCount++;
						if (dthloopCount>100)
						{break;
						}
					}
				}
			}
		}
		read[i]=data;
	}
	
	if(read[0]+read[1]+read[2]+read[3]!=read[4]){ReadHumidity=0;ReadTemp=0;}
	
	ReadHumidity= read[0]+read[1]/10;  //humidity
	
	
	ReadTemp= read[2]+read[3]/10;  //tempurarture
	
	if ((ReadHumidity>100)||(ReadTemp>40))
	{ReadHumidity=TempReadHumidity;
	ReadTemp=TempReadTemp;
	}
	
}

void ReadPH(){
	analogAV=0;
	for(uint8_t i=0;i<10;i++){
		analogAV=(ReadADC(2)/10)+analogAV;
		_delay_ms(10);
	}
	float phValueCal=(analogAV*5)/1024;
	phValue = (-5.70 *phValueCal)  + calibration_value;
}

void ReadTubidity(){
	analogtubidity=0;
	for(uint8_t i=0;i<10;i++){
		analogtubidity=(ReadADC(0)/10)+analogtubidity;
		_delay_ms(10);
	}
	 tubidity=(analogtubidity*5)/1024;
	
}

void distanceRead(){
	uint32_t disCount=0;
	PORTD&=~(1<<6);
	_delay_us(2);
	PORTD|=(1<<6);
	_delay_us(10);
	PORTD&=~(1<<6);
	while(1){
		_delay_us(1);
		disCount++;
		if (disCount>20000000)
		{break;
		}
		if (((PIND&0b10000000)>>7))
		{break;
		}
	}
	distance=disCount/148;
	
	
}

bool mainFunctionInit(){
	_delay_ms(500);
	oled_init();
	USART_Init(9600);
	PORTB|=(1<<0);// switch pull up
	DDRD|=(1<<6); //trigger pin
	DDRD&=~(1<<7);
	//SoftUART_Init();
	//SoftUSART_tx_strln("Starterd");
	InitADC();
	oled_gotoxy(0,2);
	oled_font_size(1);
	oled_write("Welcome");
	_delay_ms(1000);
	
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
	
	
	
	
	return 1;
	
}

bool checkRouter(){
	USART_TxStringln("AT+CWJAP?");
	if (USART_ReciveWait("No AP",1000))
	{
		USART_ClearRX();
		return 0;
	} 
	else
	{
		USART_ClearRX();
		return 1;
	}
	
}

bool makeHost(){
	oled_clear_screen();
	oled_gotoxy(0,2);
	oled_font_size(0);
	oled_write("Make host");
	//**********************
	USART_TxStringln("AT+CWMODE=2");
	_delay_ms(100);
	if (USART_ReciveWait("OK",500))
	{USART_ClearRX();
	}
	else
	{
		return 0;
	}
	//**********************
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
	//**********************
	USART_TxStringln("AT+CIPDINFO=0");
	_delay_ms(100);
	if (USART_ReciveWait("OK",500))
	{USART_ClearRX();
	}
	else
	{
		return 0;
	}
	//**********************
	USART_TxStringln("AT+CIPSERVER=1,80");
	_delay_ms(100);
	if (USART_ReciveWait("OK",1000))
	{USART_ClearRX();
	}
	else
	{	return 0;
	}
	USART_TxStringln("AT+CIFSR");
	_delay_ms(100);
	if (USART_ReciveWait("OK",500))
	{	
		tempcharacter=strtok(rxvalue,"\"");
		for(uint8_t i=0;i<5;i++){
			tempcharacter=strtok(NULL,"\"");
			if (strstr(tempcharacter,"192"))
			{	
				
				oled_gotoxy(0,3);
				oled_font_size(0);
				oled_write_str(tempcharacter);
				break;
			}
			if (i==4)
			{return 0;
			}
		}
	}
	else
	{
		return 0;
	}
	//******************************************
	//wait for login
	 if (USART_ReciveWait("Host"))
	 {_delay_ms(500);
		USART_ClearRX(); 
		oled_gotoxy(0,4);
		oled_write("loggin OK");
	 } 
	 else
	 {	USART_ClearRX(); 
		oled_gotoxy(0,4);
		oled_write("loggin failed");
		 return 0;
	 }
	//******************************************
	char*webpage="<!DOCTYPE html><html><body><h1>Router Configer</h1><form action=\"/action_page.php\"><label for=\"roterUsr\">Username:</label><input type=\"text\" id=\"roterUsr\" name=\"roterUsr\"><br><br><label for=\"routerPass\">Password:</label><input type=\"text\" id=\"routerPass\" name=\"routerPass\"><br><br><input type=\"submit\" value=\"Submit\"></form></body></html>\r\n";
	sprintf(lengthcal,"AT+CIPSEND=0,%d",strlen(webpage));
	USART_TxStringln(lengthcal);
	_delay_ms(500);
	USART_TxString(webpage);
	_delay_ms(500);
	//**********************
	USART_TxStringln("AT+CIPCLOSE=0");
	_delay_ms(100);
	if (USART_ReciveWait("OK",500))
	{USART_ClearRX();
	}
	else
	{
		return 0;
	}
	
	//**********************
	USART_ClearRX();
	UCSR0B&=~(1<<RXCIE0);
	char tempchar[100];
	char scanChar[4];
	uint8_t tempcharCount=0;
	while(1){
	while ( !(UCSR0A & (1<<RXC0)) ){
		
	}
	
	char Readchar=UDR0;
	for(uint8_t i=0;i<100;i++){
		tempchar[i]=tempchar[i+1];
	}
	tempchar[99]=Readchar;
	for(uint8_t i=0;i<4;i++){
		scanChar[i]=tempchar[i];
	}
	
	if (strstr(scanChar,"php?"))
	{	
		memset(wifiUsername,0,40);
		memset(wifiPassword,0,40);
		oled_gotoxy(0,4);
		oled_write("                ");
		oled_gotoxy(0,5);
		oled_write("                ");
		/*php?roterUsr=Slt-home-fwifi-dilan&routerPass=dilan12345 HTTP/1.1*/
		strncpy(wifiUsername,strstr(tempchar,"roterUsr")+9,strlen(strstr(tempchar,"roterUsr")+9)-strlen(strstr(tempchar,"&routerPass")));
		strncpy(wifiPassword,strstr(tempchar,"routerPass")+11,strlen(strstr(tempchar,"routerPass")+11)-strlen(strstr(tempchar," HTTP")));
		oled_gotoxy(0,5);
		char tempwifi[20];
		strncpy(tempwifi,wifiUsername,15);
		oled_write_str(tempwifi);
		oled_gotoxy(0,6);
		strncpy(tempwifi,wifiPassword,15);
		oled_write_str(tempwifi);
		memset(scanChar,0,4);
		memset(tempchar,0,100);//clear array
		_delay_ms(1000);
	}
	if (!(PINB&0b00000001))
	{
		break;
	}
	
	}
UCSR0B|=(1<<RXCIE0);// enable rec intruppert
//**********************************************************************
	USART_ClearRX();
	USART_TxStringln("AT+CWMODE=1");
	_delay_ms(100);
	if (USART_ReciveWait("OK",500))
	{USART_ClearRX();
	} 
	else
	{	USART_ClearRX();
		return 0;
	}
	
	
	USART_TxStringln("AT+CIPMUX=1");
	_delay_ms(100);
	if (USART_ReciveWait("OK",500))
	{
		USART_ClearRX();
		
	}
//**************************************************************
	
USART_ClearRX();

USART_TxStringln("AT+CWQAP");
if (USART_ReciveWait("OK",1000))
{
	USART_ClearRX();
	
	sprintf(tempcharacter,"AT+CWJAP=\"%s\",\"%s\"",wifiUsername,wifiPassword);
	
	USART_TxStringln(tempcharacter);
	
	if (USART_ReciveWait("GOT",10000))
	{
		oled_gotoxy(0,5);
		oled_write("Router Connected");
		USART_ClearRX();
		return 1;
	}
	else
	{	
		oled_gotoxy(0,5);
		oled_write("not connected");
		_delay_ms(2000);
		USART_ClearRX();
		return 0;
	}
	}
 }
 
bool uploaddata(){
	
	_delay_ms(500);
	USART_ClearRX();
	sprintf(lengthcal,"AT+CIPSTART=4,\"TCP\",\"%s\",80",server);
	USART_TxStringln(lengthcal);
	if (USART_ReciveWait("CONNECT",3000))
	{USART_ClearRX();
	}
	else
	{
		USART_ClearRX();
		return 0;
	}
	
	sprintf(getreqvest,"GET /naveen/dbconnect.php?waterlevel=25&pH=%.1f&temperature=%.1f&humidity=%.1f&turbidity=%.1f\r\n",phValue,ReadTemp,ReadHumidity,tubidity);
	
	
	sprintf(lengthcal,"AT+CIPSEND=4,%d\r\n",strlen(getreqvest));
	
	
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
	if (USART_ReciveWait("Insertion",2000))
	{
		_delay_ms(100);
		USART_TxString("AT+CIPCLOSE=0");
		
	}
	else
	{
		
		
		USART_ClearRX();
		return 0;
	}

	
}