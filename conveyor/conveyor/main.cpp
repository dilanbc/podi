/*
 * conveyor.cpp
 *
 * Created: 6/13/2021 7:10:50 PM
 * Author : janith
 */ 
#define F_CPU 8000000 //cpu frequancy
#include <avr/io.h>
#include <avr/interrupt.h> //parrelel process
#include <util/delay.h>  //delay
#include <avr/eeprom.h>  //eeprom
#include <stdio.h>  //printlf
#include "setjmp.h" //reset
#include "USART.h"  //gsm commnucate library
#include "LCDI2C.h"  //lcd
#include "spi.h"  //rfid
#include "RC522.h"  //rfid


uint8_t keysVal[4][3]={{1,2,3},
					{4,5,6},
					{7,8,9},
					{11,0,12}};
uint8_t eepromR[12][8]={0};		//12 card valuues			
						
						
volatile uint8_t keycheck[2]={0,0};

char lengthcal[40];//string memory
int gsm;//gsm signal calculation
char smsNumber[10]={0};
uchar status;//rfid memory
uchar str[16];//fid
jmp_buf mainstart;//error inturrept
jmp_buf midstart;//error inturrept

void PWM_init();//DC motor controlling pwm signal init
void BasicfunctionInit();//function init
void Timerinit();// timer calculation 
void EncoderInit(); 
void keyapadInit(void);
void cycle();//keypad value
bool checkGSM(); //network connect check
bool setupGSM();  
bool reading(); //rfid card id read
void cardvalueWrite(); //card write
void cardvalueRead(); 
void addlist();  //pard list adding function
void addPhoneNum();  //phone number
bool cardReadOut(); //check card list
bool otpSMS();
void RandomInit();  //otp number
void fialSMS(); //card fail
void stepperrotate(bool dir); //display
void steppergo(bool dir,uint16_t cycleCount);//stepr control
int main(void)
{
	setjmp (mainstart);//reset point
	BasicfunctionInit();
	sei();//inturrept eneble
	DDRB|=(1<<5); 
	setjmp (midstart);
	addlist();
	addPhoneNum();
	if (cardReadOut())// check added card
	{if (otpSMS())
	{stepperrotate(1);//otp ok
		steppergo(1,25000);
	} 
	else
	{stepperrotate(0);//otp fail
		steppergo(0,25000);
	}
		
	}
	else{fialSMS();steppergo(0,25000);}//
	_delay_ms(4000);
	longjmp(midstart,1);
	
    while (1) 
    {
		
    }
}

void BasicfunctionInit(){
	LcdInit();
	LcdSetCursor(4,0,"WELCOME");
	DDRB|=(1<<0)|(1<<1);// stepper motor signal dir,pulse set as output
	USART_Init(9600);//baud rate setup
	 PWM_init();//dc motor
	 Timerinit();
	 EncoderInit();
	 keyapadInit();
	 cardvalueRead();
	 RandomInit();
	 steppergo(0,25000);
	 SPI_Init();
	 MFRC522_Init();   // Initialize RC522 module
	 if (!checkGSM())
	 {longjmp(mainstart,1);
	 }
	 else{
		 LcdCommand(LCD_CLEARDISPLAY);
		 LcdSetCursor(0,0,"GSM OK");}
	 setupGSM();
	 LcdSetCursor(0,1,"GSM Initialized");
	 
}
void RandomInit(){
	srand(eeprom_read_byte((uint8_t*)97));
	eeprom_write_byte((uint8_t*)97,(eeprom_read_byte((uint8_t*)97))+1);
}
void Timerinit(){
	TCCR1B|=(1<<WGM12);
	TCCR1B|=(1<<CS11)|(1<<CS10); //prescaler 64
	OCR1A=25000;
	TIMSK1|=(1<<OCIE1A);
}
/*pid functions*/
float kP=1;
float kD=1;
float kI=5;
volatile float feedSpeed=10;
volatile float feedSpeedCal;
volatile float feedSpeedNow;
volatile int prevFeederror=0;
volatile int feedError=0;
volatile float kPchange=0;
volatile float kDchange=0;
volatile float kIchange=0;
volatile int totalFeederror=0;
volatile float PIDval=0;
ISR(TIMER1_COMPA_vect){
	PORTB^=1<<5;
	feedSpeedNow=feedSpeedCal;
	feedError=feedSpeed-feedSpeedNow;
	totalFeederror+=feedError;
	
	kPchange=feedError*kP;
	kDchange=(feedError-prevFeederror)*kD;
	kIchange=totalFeederror*kI;
	
	//PIDval=OCR2B+((kPchange+kDchange)/2);
	PIDval=(kPchange+kIchange+kDchange)/3;
	
	if (PIDval>=255)
	{OCR2B=255;
	} 
	
	else if (PIDval<=0)
	{OCR2B=0;
	}
	else
	{OCR2B=PIDval;
	}
	
	
	prevFeederror=feedError;
	feedSpeedCal=0;
	
}

void PWM_init(){
	DDRD|=(1<<3);
	TCCR2A|=(1<<COM2B1);
	TCCR2A|=(1<<WGM21)|(1<<WGM20);
	TCCR2B|=(1<<CS20);
	OCR2B=0;
}

void EncoderInit(){
	PCICR|=(1<<PCIE2);
	PCMSK2|=(1<<PCINT20);
}
ISR(PCINT2_vect){
if ((PIND&(1<<4))>>4)
{feedSpeedCal++;
}
	
}
void keyapadInit(void){
	/*write side*/
	DDRD|=(1<<5)|(1<<6)|(1<<7);
	PORTD&=~((1<<5)|(1<<6)|(1<<7));

	/*read side*/
	DDRC&=~((1<<3)|(1<<2)|(1<<1)|(1<<0));
	PORTC|=(1<<3)|(1<<2)|(1<<1)|(1<<0);
	
	PCICR|=(1<<PCIE1);
	PCMSK1|=(1<<PCINT11)|(1<<PCINT10)|(1<<PCINT9)|(1<<PCINT8);
}

ISR(PCINT1_vect){
	
	
	if (!((PINC&(1<<0))>>0))
	{keycheck[0]=0;
		cycle();
	}
	else if (!((PINC&(1<<1))>>1))
	{keycheck[0]=1;
		cycle();
	}
	else if (!((PINC&(1<<2))>>2))
	{keycheck[0]=2;
		cycle();
	}
	else if(!((PINC&(1<<3))>>3))
	{keycheck[0]=3;
		cycle();
	}
	
	
	
	
}


volatile uint8_t pinj;
volatile uint8_t nowKey=0;
volatile bool keyState=0;
void cycle(void){
	DDRC|=(1<<3)|(1<<2)|(1<<1)|(1<<0);
	PORTC&=~((1<<3)|(1<<2)|(1<<1)|(1<<0));
	
	DDRD&=~((1<<5)|(1<<6)|(1<<7));
	PORTD|=(1<<5)|(1<<6)|(1<<7);
	
	pinj=PIND;pinj=PIND;pinj=PIND;
	
	
	
	if (!((PIND&(1<<5))>>5))
	{keycheck[1]=0;
	}
	else if (!((PIND&(1<<6))>>6))
	{keycheck[1]=1;
	}
	else if (!((PIND&(1<<7))>>7))
	{keycheck[1]=2;
	}
	
	nowKey=keysVal[keycheck[0]][keycheck[1]];
	keyState=1;
	/*defult*/
	/*write side*/
	DDRD|=(1<<5)|(1<<6)|(1<<7);
	PORTD&=~((1<<5)|(1<<6)|(1<<7));

	/*read side*/
	DDRC&=~((1<<3)|(1<<2)|(1<<1)|(1<<0));
	PORTC|=(1<<3)|(1<<2)|(1<<1)|(1<<0);
	
}	

bool checkGSM(){
	DDRD|=(1<<2);
	PORTD|=(1<<2);
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
	USART_TxStringln("AT+CPIN?");//sim check
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
	USART_TxStringln("AT+CSQ");  //signal strength
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
	
	USART_TxStringln("AT+CNMI=1,2,0,0,0");//sms read
	USART_ReciveWait("OK",1000,1);
	_delay_ms(200);
	USART_TxStringln("AT+DDET=1,0,0");
	USART_ReciveWait("OK",1000,1);
	_delay_ms(200);
	USART_ClearRX();
	
	//****Data function
	
	
}

bool reading(){
	bool state=0;
	uint8_t ReadingCard [8]={};
	
	//Find cards, return card type - Don't remove this sub
	status = MFRC522_Request(PICC_REQIDL, str);
	//Anti-collision, return card serial number 4 bytes
	status = MFRC522_Anticoll(str);
	memcpy(serNum, str, 5);
	
	if (status == MI_OK)   //if ther is a new card detected
	{
		
		SetFormatRDM630(); // take serNum and format it and retern read
		for(uint8_t i=0;i<8;i++){
			ReadingCard[i]=read[i];
			
		}

		
		
	}
	return state;
}
uint8_t j;
void cardvalueWrite(){
	/*for(uint8_t i=0;i<96;i++){
		eeprom_write_byte((uint8_t*)i,0);
	}*///clear
	for( j=0;j<12;j++){
	while(1){
		status = MFRC522_Request(PICC_REQIDL, str);
		//Anti-collision, return card serial number 4 bytes
		status = MFRC522_Anticoll(str);
		memcpy(serNum, str, 5);

		if (status == MI_OK)   //if ther is a new card detected
		{
			SetFormatRDM630(); // take serNum and format it and retern read
			for(uint8_t i=0;i<8;i++){
				eeprom_write_byte((uint8_t*)((8*j)+i),read[i]);
				
			}
			sprintf(lengthcal,"val %u",j);
			LcdSetCursor(0,0,lengthcal);
			LcdCommand(LCD_CLEARDISPLAY);
			break;
			
		}
		
	}	

}
	
}
void cardvalueRead(){
	for(uint8_t i=0;i<12;i++){
		for(uint8_t j=0;j<8;j++){
			eepromR[i][j]=eeprom_read_byte((uint8_t*)(8*i+j));
		}
	}
}
volatile uint8_t addlistnum[4]={0,0,0,0};
void addlist(){
	addlistnum[0]=0;addlistnum[1]=0;addlistnum[2]=0;addlistnum[3]=0;
	keyState=0;
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(1,0,"ADD PART LIST");
	_delay_ms(1000);
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"A-1 B-2 C-3 D-4");
	LcdSetCursor(0,1,"*-clear #-enter");
	_delay_ms(3000);
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(2,0,"A  B  C  D ");
	char lcdt[16];
	sprintf(lcdt,"%u  %u  %u  %u",addlistnum[0],addlistnum[1],addlistnum[2],addlistnum[3]);
	LcdSetCursor(2,1,lcdt);
	while(1){
		if (keyState)
		{	switch(nowKey){
			case 1: addlistnum[0]+=1;if (addlistnum[0]>3)
			{addlistnum[0]=0;
			}break;
			
			case 2: addlistnum[1]+=1;if (addlistnum[1]>3)
			{addlistnum[1]=0;
			}break;
			
			case 3: addlistnum[2]+=1;if (addlistnum[2]>3)
			{addlistnum[2]=0;
			}break;
			
			case 4: addlistnum[3]+=1;if (addlistnum[3]>3)
			{addlistnum[3]=0;
			}break;
			
			case 11:addlistnum[0]=0; addlistnum[1]=0;addlistnum[2]=0;addlistnum[3]=0;break;
			
			}
			sprintf(lcdt,"%u  %u  %u  %u",addlistnum[0],addlistnum[1],addlistnum[2],addlistnum[3]);
			LcdSetCursor(2,1,lcdt);
			keyState=0;
			
			if (nowKey==12)
			{break;
			}
		}
	}
	keyState=0;nowKey=0;
	LcdCommand(LCD_CLEARDISPLAY);
	
}
uint8_t phoneNum[8]={0,0,0,0,0,0,0,0};
void addPhoneNum(){
	keyState=0;
	uint8_t lcdPosition=0;
	char lcdt[1];
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0," Mobile Number");
	LcdSetCursor(1,1,"07");
	LcdCommand(LCD_DISPLAYCONTROL|(LCD_DISPLAYON|LCD_BLINKON));
	while(1){
		if (keyState)
		{	
			if ((nowKey<10)&&(lcdPosition<8))
			{	sprintf(lcdt,"%u",nowKey);
				LcdSetCursor(3+lcdPosition,1,lcdt);
				phoneNum[lcdPosition]=nowKey;
				lcdPosition++;
			}
			
			if ((nowKey==11)&&(lcdPosition>0))
			{	lcdPosition--;
				LcdSetCursor(3+lcdPosition,1," ");
				LcdSetCursor(3+lcdPosition,1,"");
			}
			
			if (lcdPosition>7)
			{LcdCommand(LCD_DISPLAYCONTROL|(LCD_DISPLAYON&~LCD_BLINKON));
			}
			else{LcdCommand(LCD_DISPLAYCONTROL|(LCD_DISPLAYON|LCD_BLINKON));}
			
		keyState=0;
	}
	if ((nowKey==12)&&(lcdPosition>7))
	{break;
	}
	}
	keyState=0;
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0," Entered Number");
	
	sprintf(smsNumber,"07%u%u%u%u%u%u%u%u",phoneNum[0],phoneNum[1],phoneNum[2],phoneNum[3],phoneNum[4],phoneNum[5],phoneNum[6],phoneNum[7]);
	LcdSetCursor(2,1,smsNumber);
	_delay_ms(500);
}

uint8_t addlistnumnow[4]={0};
bool cardReadOut(){
	_delay_ms(100);
	addlistnumnow[0]=0;addlistnumnow[1]=0;addlistnumnow[2]=0;addlistnumnow[3]=0;
	keyState=0;
	LcdCommand(LCD_CLEARDISPLAY);
	char lcdt[16]={0};
	sprintf(lcdt,"  A%u B%u C%u D%u",addlistnum[0],addlistnum[1],addlistnum[2],addlistnum[3]);
	LcdSetCursor(0,0,lcdt);
	
	sprintf(lcdt,"  %u  %u  %u  %u",addlistnumnow[0],addlistnumnow[1],addlistnumnow[2],addlistnumnow[3]);
	LcdSetCursor(0,1,lcdt);
	volatile uint8_t findCard=5;
	volatile bool cardReadState[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	while(1){
		

		status = MFRC522_Request(PICC_REQIDL, str);
		//Anti-collision, return card serial number 4 bytes
		status = MFRC522_Anticoll(str);
		memcpy(serNum, str, 5);

		if (status == MI_OK)   //if ther is a new card detected
		{
			SetFormatRDM630(); // take serNum and format it and return read
			volatile uint8_t rfup=0;
			 findCard=5;
			for(uint8_t i=0;i<12;i++){
				
			for(uint8_t k=0;k<8;k++){
				if (read[k]==eepromR[i][k])
				{
					rfup++;
				}
				else{
					rfup=0;
				}
				if ((rfup==8)&&(!cardReadState[i]))
				{	findCard=i/3;
					cardReadState[i]=1;
					break;
				}
			}//end j
			if (findCard!=5)
			{break;
			}
				
			}//end i
			if (findCard<4)
			{
			addlistnumnow[findCard]+=1;
			sprintf(lcdt,"  %u  %u  %u  %u",addlistnumnow[0],addlistnumnow[1],addlistnumnow[2],addlistnumnow[3]);
			LcdSetCursor(0,1,lcdt);
			_delay_ms(2000);
			steppergo(1,4000);
			steppergo(0,5000);	
			}
			findCard=5;
		}//if
		if ((nowKey==12)&&(keyState))
		{break;
		}
	}//while
	volatile uint8_t numcheck=0;
	for(uint8_t i=0;i<4;i++){
	if (addlistnum[i]==addlistnumnow[i])
	{numcheck++;
	} 
	else
	{break;
	}
	}//for
	if (numcheck>3)
	{	LcdCommand(LCD_CLEARDISPLAY);
		LcdSetCursor(0,0,"Cards are Accepted");
		return 1;
	} 
	else
	{	
		LcdCommand(LCD_CLEARDISPLAY);
		LcdSetCursor(0,0," items Rejected");
		LcdSetCursor(2,1,"");
		for(uint8_t i=0;i<4;i++){
			if (addlistnum[i]==addlistnumnow[i])
			{
			}
		}
		return 0;
		
	}
}

bool otpSMS(){
	sprintf(lengthcal,"AT+CMGS=\"%s\"\r\n",smsNumber);
	USART_TxString(lengthcal);
	_delay_ms(200);
	uint16_t randomNum=(rand()%(9999-1000+1))+1000;
	sprintf(lengthcal,"Your item list is\r\nA-%u\r\nB-%u\r\nC-%u\r\nD-%u\r\n OTP is %u ",addlistnumnow[0],addlistnumnow[1],addlistnumnow[2],addlistnumnow[3],randomNum);
	USART_TxString(lengthcal);
	_delay_ms(500);
	USART_Transmit(26);
	
	keyState=0;
	uint8_t lcdPosition=0;
	char lcdt[2];
	volatile uint8_t otpnum[4]={0};
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Enter the OTP");
	LcdCommand(LCD_DISPLAYCONTROL|(LCD_DISPLAYON|LCD_BLINKON));
	LcdSetCursor(1,2,"");
	while(1){
		if (keyState)
		{
			if ((nowKey<10)&&(lcdPosition<4))
			{	sprintf(lcdt,"%u",nowKey);
				LcdSetCursor(3+lcdPosition,1,lcdt);
				otpnum[lcdPosition]=nowKey;
				lcdPosition++;
			}
			
			if ((nowKey==11)&&(lcdPosition>0))
			{	lcdPosition--;
				LcdSetCursor(3+lcdPosition,1," ");
				LcdSetCursor(3+lcdPosition,1,"");
			}
			
			if (lcdPosition>3)
			{LcdCommand(LCD_DISPLAYCONTROL|(LCD_DISPLAYON&~LCD_BLINKON));
			}
			else{LcdCommand(LCD_DISPLAYCONTROL|(LCD_DISPLAYON|LCD_BLINKON));}
			
			keyState=0;
		}
		if ((nowKey==12)&&(lcdPosition>3))
		{break;
		}
	}
	
	uint16_t otpcheck=(otpnum[0]*1000)+(otpnum[1]*100)+(otpnum[2]*10)+(otpnum[3]);
	if (otpcheck==randomNum)
	{return 1;
	} 
	else
	{return 0;
	}
}

void fialSMS(){
sprintf(lengthcal,"AT+CMGS=\"%s\"\r\n",smsNumber);
USART_TxString(lengthcal);
_delay_ms(200);
sprintf(lengthcal,"Order rejected.\r\n Your item list is\r\nA-%u\r\nB-%u\r\nC-%u\r\nD-%u\r\n ",addlistnumnow[0],addlistnumnow[1],addlistnumnow[2],addlistnumnow[3]);
USART_TxString(lengthcal);
_delay_ms(500);
USART_Transmit(26);
_delay_ms(2000);	
	
}

void stepperrotate(bool dir){
	if (dir)
	{LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"OTP Correct");
	} 
	else
	{LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"OTP Incorrect");
	}
}

void steppergo(bool dir,uint16_t cycleCount){
	if (dir)
	{PORTB|=(1<<0);// dir pin high
	} 
	else
	{PORTB&=~(1<<0);// dir pin low
	}
	
	for(uint16_t i=0;i<cycleCount;i++){
		PORTB|=(1<<1);
		_delay_us(200);
		PORTB&=~(1<<1);
		_delay_us(200);
	}
}