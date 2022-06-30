/*
 * hybrid.cpp
 *
 * Created: 8/12/2020 4:57:37 PM
 * Author : Dilan
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <setjmp.h>
#include <stdio.h>
#include "I2C.h"
#include "LCDI2C.h"
#include "USART.h"



jmp_buf env;
bool mode=0;  // cycle mode
uint16_t timerCount;
uint16_t count;
uint16_t timelimit;

/*timer mode*/
#define  timeranalogPins 8
#define  timerEvent 2
uint16_t timerBox [timeranalogPins][timerEvent]={};
uint16_t bitTimeCount;

void timerMode();
void cycleMode();
void adcInit();
float readAdc(uint8_t channel);
void chargeRelay(uint8_t RelayNumber,bool RelayState);
void disRelay(uint8_t RelayNumber,bool RelayState);
char fValue[10];

int main(void)
{
	USART_Init(9600);
	LcdInit();
	adcInit();
	USART_TxStringln("Started");
	mode=eeprom_read_byte((uint8_t)0);
	
	PIND|=(1<<4); //PIND 4 as led
	PORTD&=~(1<<4);// off led
	
	/*Mode pin Setting*/
	PIND&=~(1<<2);//PD2 Selector switch as input
	PORTD|=(1<<2);//Pull up
	MCUCR|=(1<<ISC01); //falling edge interrupt
	MCUCSR|=(1<<ISC2); //pulse length 50ms
	GICR|=(1<<INT0); //enable interrupt
	
	/*Start pin setting  int1*/ 
	PIND&=~(1<<3);//PD3 Selector switch as input
	PORTD|=(1<<3);//Pull up
	
	/*Enter button*/
	PIND&=~(1<<0);//PD3 Selector switch as input
	PORTD|=(1<<0);//Pull up
	
	/*Timer Setting*/
	TCCR0|=(1<<CS02)|(1<<CS00); //preScaler =1024 ,64us
	//100ms/64us=1562.5
	OCR0 =157;
	
	sei();		//enable Interrupt
	DDRB=0xFF;  //relay pins as outputs
	DDRC|=(0b00111111<<2);
	DDRD|=(0b11<<6);
	if (setjmp (env))
	{}
	TIMSK&=~(1<<OCIE1A);	
	bitTimeCount=0;
	LcdInit();	
	LcdSetCursor(0,0,"Plz Select mode");
	if (mode==0)
	{LcdSetCursor(3,1,"Cycle Mode");
	} 
	else
	{LcdSetCursor(1,1,"HV BTRY Report");
	}
	
	
		//USART_TxStringln("Double cycle");...........................................................
		
//		
	while (1) 
    {	for(uint8_t i=0;i<8;i++){disRelay(i,0);chargeRelay(i,0);}
	
		while(((PIND&(1<<3))>0)){}
		
	if (mode==0)
    {cycleMode();
    } 
    else
    {timerMode();
    }
		
    }
}

ISR(INT0_vect){
	USART_TxStringln("pressed");
	
	GICR&=~(1<<INT0);		//disable interrupt
	TIMSK|=(1<<OCIE0);  //enable timer
	timelimit=100;
}

ISR(TIMER0_COMP_vect){
	TCNT0=0;
	timerCount++;
	if(((PIND&(1<<2))==0)){
		count++;
		}
	if (count>timelimit)
	{	mode=!mode;
		count=0;
		PORTD^=(1<<4);// toggle
		eeprom_write_byte((uint8_t)0,mode);
		USART_TxString("Mode Changed");
		USART_TxNumber(mode);
					
		
	} 
	if (timerCount>count+timelimit)
	{GICR|=(1<<INT0);		//enable interrupt
	TIMSK&=~(1<<OCIE0);  //disable timer
	timerCount=0;
	count=0;
	longjmp (env, 1);
	}
	/*USART_TxNumber(count);
	USART_TxStringln("");*/
		
	
}

void timerMode(){
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(1,0,"Start Report.");
	for(uint8_t i=0;i<timeranalogPins;i++){timerBox[i][0]=0;timerBox[i][1]=0;} //Clear timer box
	_delay_ms(1000);
	while(((PIND&(1<<3))>0)){}
		
		_delay_ms(500);
		for(uint8_t i=0; i<8;i++){
			disRelay(i,1);
		}
	
	//..................................................................................................//
	
	OCR1A=62500;
	TCCR1A|=(1<<WGM12); //ctc mode
	TIMSK|=(1<<OCIE1A);  //intrppet enable
	TCCR1B|=(1<<CS12); //prescaler is 256
	sei();
	
	uint8_t now=0;
	LcdCommand(LCD_CLEARDISPLAY);
	char buf[16]={};
	sprintf(buf,"%d Top",now+1);
	LcdSetCursor(0,0,buf);
	sprintf(buf,"%dm",timerBox[now][0]/60);
	LcdSetCursor(10,0,buf);
	sprintf(buf,"%ds",timerBox[now][0]%60);
	LcdSetCursor(13,0,buf);
	//
	LcdSetCursor(2,1,"Bottom");
	sprintf(buf,"%dm",timerBox[now][1]/60);
	LcdSetCursor(10,1,buf);
	sprintf(buf,"%ds",timerBox[now][1]%60);
	LcdSetCursor(13,1,buf);
	_delay_ms(500);
	
	while(1){
		if (((PIND&(1<<0))==0))
	{LcdCommand(LCD_CLEARDISPLAY);
		
		now++;
		if (now==7)
		{now=0;
		}
		char buf[16]={};
		sprintf(buf,"%d Top",now+1);
		LcdSetCursor(0,0,buf);
		sprintf(buf,"%dm",timerBox[now][0]/60);
		LcdSetCursor(10,0,buf);
		sprintf(buf,"%ds",timerBox[now][0]%60);
		LcdSetCursor(13,0,buf);
		//
		LcdSetCursor(2,1,"Bottom");
		sprintf(buf,"%dm",timerBox[now][1]/60);
		LcdSetCursor(10,1,buf);
		sprintf(buf,"%ds",timerBox[now][1]%60);
		LcdSetCursor(13,1,buf);
		_delay_ms(500);
	}//if
	if (((PIND&(1<<3))==0))
	{break; //start button stop
	}
	
	}
	
	_delay_ms(500);
	bitTimeCount=0;
	TIMSK&=~(1<<OCIE1A);
	longjmp (env, 1);
}
ISR(TIMER1_COMPA_vect){
TCNT1=0;	

/*
char  newbf[20];
dtostrf(readAdc(1),4,3,newbf);
USART_TxStringln(newbf);*///,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
for(uint8_t i=0;i<8;i++){
	
	if ((readAdc(i)<7)&&(timerBox[i][0]==0))
	{timerBox[i][0]=bitTimeCount;
	}
	if ((readAdc(i)<6.5)&&(timerBox[i][1]==0))//.............................................
	{timerBox[i][1]=bitTimeCount;
		timerBox[i][1]=bitTimeCount-timerBox[i][0];
		disRelay(i,0);
	}
}
bitTimeCount++;

}

void cycleMode(){
	uint8_t voltageMode=0;
	float voltage=3;
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(1,0,"Select Voltage");
	LcdSetCursor(5,1,"3.0V");
	_delay_ms(1000);
	while(((PIND&(1<<3))>0)){
		if (((PIND&(1<<0))==0))
		{				//voltage select
			_delay_ms(500);
			voltageMode++;
			if (voltageMode>3)
			{voltageMode=0;
			}
		switch(voltageMode){ // start switch
			case 0: LcdSetCursor(5,1,"3.0V"); break;
			case 1: LcdSetCursor(5,1,"4.5V"); break;
			case 2: LcdSetCursor(5,1,"5.5V"); break;
			case 3: LcdSetCursor(5,1,"6.5V"); break;
		}// end of switch
		
		}//end voltage select
		if (((PIND&(1<<3))==0))
		{break;
		}
	}//while
	/*Start Discharging and charging*/
	LcdCommand(LCD_CLEARDISPLAY);
	switch(voltageMode){ // start switch
		case 0: LcdSetCursor(0,0,"Cycle Processing");voltage=3;USART_TxStringln("3.0V"); break;
		case 1: LcdSetCursor(0,0,"Cycle Processing");voltage=4.5;USART_TxStringln("4.5V"); break;
		case 2: LcdSetCursor(0,0,"Cycle Processing");voltage=5.5; USART_TxStringln("5.5V");break;
		case 3: LcdSetCursor(0,0,"Cycle Processing");voltage=6.5;USART_TxStringln("6.5V"); break;
	}// end of switch
	
	
/*
	
	0		1		2		3		4		5
	D		C		D		C		D		C  
A0	0,0		0,1		0,2		0,3		0,4		0,5
A1	1,0		1,1		1,2		1,3		1,4		1,5
A2	2,0		2,1		2,2		2,3		2,4		2,5
A3	3,0		3,1		3,2
A4	4,0		4,1		4,2
A5	5,0		5,1		5,2
A6	6,0		6,1		6,2
A7	7,0		7,1		7,2
	
	*/

/*Discharging
Dis0	Dis1	Dis2	Dis3	Dis4	Dis5	dis6	Dis7
PORTB0	PORTB1	PORTB2	PORTB5	PORTB4	PORTB5	PORTB6	PORTB7*/

/*
Charging
Char0	Char1	Char2	Char3	Char4	Char5	Char6	Char7
PD6		PD7		PC2		PC3		PC4		PC5		PC6		PC7*/

	uint8_t analogPins=8;
	uint8_t cycles=6;
	bool cycleBox[analogPins][cycles]={};
	bool completeBox[analogPins]={};
	LcdSetCursor(3,1,"");
	while((completeBox[0]&&completeBox[1]&&completeBox[2]&&completeBox[3]&&completeBox[4]&&completeBox[5]&&completeBox[6]&&completeBox[7])==0){
		
	for(uint8_t i=0; i<analogPins;i++){ // vertical for
		dtostrf(readAdc(i),3,2,fValue);
	/*	for(uint8_t m=0;m<6;m++){USART_TxNumber(cycleBox[i][m]);} 
			USART_TxString("  ");
			 USART_TxStringln(fValue);*/
			
		for(uint8_t j=0;j<cycles;j++){  // horizontal for
			if (cycleBox[i][j]==0) // check 1
			{
				if (j%2==0)//even
					{
					if (readAdc(i)>voltage) //discharging
					{disRelay(i,1);
					} 
					else
					{disRelay(i,0);cycleBox[i][j]=1;
					}
				} //end even
				else //odd
				{
					if (readAdc(i)<8.4) //charging
					{chargeRelay(i,1);
					}
					else
					{chargeRelay(i,0);cycleBox[i][j]=1;
					}
					
				}  //end ood
				break;
			}// end check 1
		}// end horizontal for
		
		if ((cycleBox[i][5]==1)&&(completeBox[i]==0))
		{char buf[20]={};
		sprintf(buf,"%d Completed\r\n",i+1);
		USART_TxString(buf);
		
		sprintf(buf,"%d",i+1);
		completeBox[i]=1;
		LcdString(buf);
		}
	}// end of vertical for
	//_delay_ms(100); //.............................................................................Remove
	/*USART_TxStringln("");
	USART_TxStringln("");
	USART_TxStringln("");
	*/
	
	
}// end of while
while (((PIND&(1<<3))>0))
_delay_ms(500);
longjmp (env, 1);
	
}

void adcInit(){
	ADMUX|=1<<REFS0;  // reference AVCC
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //enable adc prescaler is 128
	
}

float readAdc(uint8_t channel){
	ADMUX=(ADMUX&0xf0)|(0b111&channel);
	ADCSRA|=(1<<ADSC);   // start conversion
	while(!(ADCSRA & (1<<ADIF)));   // wait for ADIF conversion complete return
	ADCSRA|=(1<<ADIF);   // clear ADIF when conversion complete by writing 1
	return ((ADC/102.4)+0.1); //return calculated ADC value
	
}

void disRelay(uint8_t RelayNumber,bool RelayState){
	
		if (RelayState==0)
		{PORTB&=~(1<<RelayNumber);
		} 
		else
		{PORTB|=(1<<RelayNumber);
		}
	
	
}

void chargeRelay(uint8_t RelayNumber,bool RelayState){
	if (RelayState==0)
	{	switch(RelayNumber){
		case 0:PORTD&=~(1<<6); break;
		case 1:PORTD&=~(1<<7); break;
		case 2:PORTC&=~(1<<RelayNumber); break;
		case 3:PORTC&=~(1<<RelayNumber); break;
		case 4:PORTC&=~(1<<RelayNumber); break;
		case 5:PORTC&=~(1<<RelayNumber); break;
		case 6:PORTC&=~(1<<RelayNumber); break;
		case 7:PORTC&=~(1<<RelayNumber); break;
		
	}
	} 
	else
	{
		switch(RelayNumber){
			case 0:PORTD|=(1<<6); break;
			case 1:PORTD|=(1<<7); break;
			case 2:PORTC|=(1<<RelayNumber); break;
			case 3:PORTC|=(1<<RelayNumber); break;
			case 4:PORTC|=(1<<RelayNumber); break;
			case 5:PORTC|=(1<<RelayNumber); break;
			case 6:PORTC|=(1<<RelayNumber); break;
			case 7:PORTC|=(1<<RelayNumber); break;
			
		}
	}
}