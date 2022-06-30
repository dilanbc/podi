/*
 * feeder.cpp
 *
 * Created: 3/4/2021 10:28:14 PM
 * Author : Dilan
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h> 
#include <setjmp.h>
#include "USART.h"
#include "LCDI2C.h"

jmp_buf env;

/*xSide Variables*/
volatile int16_t xposition=0;
volatile uint8_t prevx=0;
bool first_x=0;
float x_max_speed;  //40max
int32_t XA;
int16_t XT;
float xP=4;  //4
float xI=0.2; //0.2
float xD=5; //5
int16_t prevXerror;
int16_t xMotor=170;
volatile bool xEnable=0;

/*ySide Variable*/
volatile uint32_t yposition=0;
float y_acc;		//0.5max
float y_max_speed;  //40max
uint32_t A1;
uint32_t A2;
volatile int32_t TimerCount;
uint16_t T1;
uint16_t T2;
float yP=0.4;  //0.4
float yI=0.05; //0.05
float yD=3;
int16_t prevYerror;
int32_t errorI;
int16_t yMotor=170;
volatile bool yEnable=0;
#define yrate 289

float length=0;
uint16_t count=0;
uint8_t lcddataRead[5]={};	

//int16_t temp1[600];
uint16_t tempi=0;
float tempu8;
char tempchrt[20];
void go_x(uint8_t max_speed,int32_t limit);

void InitADC()
{
	// Select Vref=AVcc
	ADMUX |= (1<<REFS0);
	//set prescaller to 128 and enable ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	DDRC|=(1<<0);
	PORTC|=(1<<0);
	
	DDRD&=~(1<<0);
	PORTD|=(1<<0);
	DDRB|=(1<<3);  // rely
	
	//0- joy push button
	//1- joy x
	//2-joy y
	//3-potentiol
	//pd0 main push button
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

void xaxisInit(){
	
	/*PD2 and PD3*/
	DDRD&=~(1<<2);
	DDRD&=~(1<<3);
	
	/*pullup*/
	PORTD|=(1<<2);
	PORTD|=(1<<3);
	PORTD|=(1<<4);// limit switch pul up
	
	/*interrupt enable process-PCINT18,PCINT19*/
	PCICR|=(1<<PCIE2); //Pin Change Interrupt Enable 2
	PCMSK2|=(1<<PCINT18);
	PCMSK2|=(1<<PCINT19);
	PCMSK2|=(1<<PCINT20);// for limit switch
	
	
	
	
	
	
}

ISR(PCINT2_vect){  //xaxis
	/*0 2 3 1 0 2 3 1*/
	uint8_t in_x=(PIND&0b00001100)>>2;
	if (first_x==0)
	{prevx=in_x;
		first_x=1;
	} 
	else
	{
		
		bool dir_x=0;
		
		
		switch(in_x){
			case 0 : if (prevx==1){dir_x=1;} break;
			case 2 : if (prevx==0){dir_x=1;} break;
			case 3 : if (prevx==2){dir_x=1;} break;
			case 1 : if (prevx==3){dir_x=1;} break;
			
		}
		if (dir_x==1)///////////////    direction change
		{ xposition++;
		}
		else
		{xposition--;
		}
		prevx=in_x;
		
		
	}
	
	if (((PIND&0b00010000)>>4)==0)
	{
	
	TimerCount=0;
	XT=0;
	OCR0B=0;
	TCCR1B=0;
	xEnable=0;
	
	go_x(10,500);
	
	xposition=0;
	}
	
	if (xposition>6000)
	{
		
		TimerCount=0;
		XT=0;
		OCR0B=0;
		TCCR1B=0;
		xEnable=0;
		_delay_ms(1000);
		xposition=6000;
	}
	

}

void timerInit(){
	TCCR1B|=(1<<WGM12);
	TCCR1B|=(1<<CS11)|(1<<CS10); //prescaler =8
	OCR1A=1250;
	TIMSK1|=(1<<OCIE1A);
}

void go_x(uint8_t max_speed,int32_t limit){
	if (limit<0)
	{PORTB|=(1<<7);
		PORTB&=~(1<<6);
	}
	else
	{PORTB|=(1<<6);PORTB&=~(1<<7);
	}
	
	timerInit();
	TimerCount=0;
	xposition=0;
	errorI=0;
	xMotor=0;
	
	x_max_speed=max_speed;
	XT=limit/max_speed;

	OCR0B = 0;
	
	
	TimerCount=0;
	xEnable=1;
	
	
	
	
	
	
}


void yaxisInit(){
	/*PB1 and PB2*/
	DDRB&=~(1<<1);
	DDRB&=~(1<<2);
	
	/*pullup*/
	PORTB|=(1<<1);
	PORTB|=(1<<2);
	
	/*interrupt enable process-PCINT1,PCINT2*/
	PCICR|=(1<<PCIE0); //Pin Change Interrupt Enable 0
	PCMSK0|=(1<<PCINT1);
	PCMSK0|=(1<<PCINT2);
	
	DDRB|=(1<<0);
	
}

ISR(PCINT0_vect){
	
	
	if (((PINB&0b00000110)>>1)==0)
	{yposition++;
	}
	
	

}

void axisControlInit(){
	 DDRD |= (1 << 6);
	 DDRD |= (1 << 5);
	 // PD6 and PD5 is now an output

	 OCR0A = 0;
	 OCR0B=0;
	 // set PWM for 50% duty cycle


	 TCCR0A |= (1 << COM0A1)|(1<<COM0B1);
	 // set none-inverting mode

	 TCCR0A |= (1 << WGM01) | (1 << WGM00);
	 // set fast PWM Mode

	 TCCR0B |= (1 << CS01);
	 // set prescaller to 8 and starts PWM
	 
	 DDRB |= (1 << 6); // x dir control
	 DDRB |= (1 << 7);
	 
	 DDRD|=(1<<7);  //y dir control
	 DDRB|=(1<<0);
	 
}

void go_y(bool dir,float acc,uint8_t max_speed,uint32_t limit){
	timerInit();
	yposition=0;
	errorI=0;
	yMotor=0;
	if (dir)
	{PORTB|=(1<<0);
	}
	else
	{}
	A1=0.5*((pow(max_speed,2))/acc);
	
	if ((limit)>(2*A1))
	{	A2=limit-(2*A1);
		T1=sqrt((2*A1)/acc);
		T2=A2/max_speed;
		y_acc=acc;
		y_max_speed=max_speed;
		OCR0A = 0;
	} 
	else
	{
		T1=sqrt(limit/acc);
		T2=0;
		A1=limit/2;
		A2=0;
		y_acc=acc;
		y_max_speed=y_acc*T1;
		OCR0A = 0;
	}
	TimerCount=0;
	yEnable=1;
	
}




ISR(TIMER1_COMPA_vect){
	/*y axis part start*/
	if (yEnable)
	{
		TimerCount++;
	if (TimerCount<T1)
	{
		uint16_t predictA=0.5*y_acc*(pow(TimerCount,2));
		int16_t error=predictA-yposition;
		errorI+=error;
		yMotor=(errorI*yI)+(error*yP)+(yD*(error-prevYerror));
		prevYerror=error;
		if (yMotor>=255)
		{OCR0A=255;
		}
		else{
			OCR0A=yMotor;
		}
				
	} 
	
	else if((TimerCount>=T1)&&(TimerCount<(T1+T2)))
	{
	uint16_t predictA=A1+((TimerCount-T1)*y_max_speed);
	int16_t error=predictA-yposition;
	errorI+=error;
	yMotor=(errorI*yI)+(error*yP)+(yD*(error-prevYerror));
	prevYerror=error;
	
	if (yMotor>=255)
	{OCR0A=255;
	}
	else{
		OCR0A=yMotor;
	}	
	
	}
	
	
	
	else if ((TimerCount>=(T1+T2))&&(TimerCount<((2*T1)+T2))){
			
		uint16_t predictA=A1+A2+(0.5*((2*y_max_speed)-(y_acc*(TimerCount-(T1+T2)))))*(TimerCount-(T1+T2));
		int16_t error=predictA-yposition;
		errorI+=error;
		yMotor=(errorI*yI)+(error*yP)+(yD*(error-prevYerror));
		prevYerror=error;
		
		if (yMotor>=255)
		{OCR0A=255;
		}
		else{
			OCR0A=yMotor;
		}
		/*temp1[tempi]=error;
		tempi++;	*/
				
	}
	else{
	/*for(uint16_t i=0;i<600;i++){
		sprintf(tempchrt,"%d\r\n",temp1[i]);
		USART_TxString(tempchrt);
	}*/
	/*sprintf(tempchrt,"%u\r\n",yposition);
	USART_TxString(tempchrt);*/
	yEnable=0;
	OCR0A = 0;
	
	TCCR1B=0;
	
	yposition=0;

	
	
	}
		
		
		
		
	}
/*end Y Enable	*/


/*X enable*////////////////////////////////////////////////////////////////////////////////////


if (xEnable)
{
	TimerCount++;
	
	if (XT>0)
	{
		
		if (TimerCount<XT)
	{
		
		int32_t predictA=x_max_speed*TimerCount;
		int16_t error=predictA-xposition;
		errorI+=error;
		xMotor=(errorI*xI)+(error*xP)+(xD*(error-prevXerror));
		prevXerror=error;
		if (xMotor>=255)
		{OCR0B=255;
		}
		else{
			OCR0B=xMotor;
		}
	
	} 
	else
	{xEnable=0;
		OCR0B=0;
		TCCR1B=0;
		
	}
	} 
	else// for - axis
	{
		if (TimerCount<abs(XT))
		{
			int32_t predictA=x_max_speed*TimerCount;
			int16_t error=predictA+xposition;
			errorI+=error;
			xMotor=(errorI*xI)+(error*xP)+(xD*(error-prevXerror));
			prevXerror=error;
			if (xMotor>=255)
			{OCR0B=255;
			}
			else{
				OCR0B=xMotor;
			}
		} 
		else
		{xEnable=0;
		OCR0B=0;
		TCCR1B=0;
		}
	}
	
	
	
	
	
}
 	
	
	
	

}

bool Modeselect(){
	bool logic=0;
	LcdSetCursor(3,0,"Select Mode");
	LcdSetCursor(0,1," >Auto   Manual");
	while(((PINC&0b1)>>0)!=0){
		if (ReadADC(1)<50)
		{LcdSetCursor(1,1,">");
			LcdSetCursor(8,1," ");
			logic=0;
		}
		if (ReadADC(1)>1000)
		{LcdSetCursor(1,1," ");
			LcdSetCursor(8,1,">");
			logic=1;
		}
	}
	return logic;
	
}

void ManualMode(){
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(3,0,"Manual mode");
	_delay_ms(500);
	
	while(1){
		if (ReadADC(1)>530)
		{
			PORTB|=(1<<7);
			PORTB&=~(1<<6);
			tempi=ReadADC(1);
			tempu8=(((tempi-530)*85)/(1023-530))+150;
			OCR0B= tempu8;
			
		}
		else if (ReadADC(1)<520)
		{
			PORTB|=(1<<6);
			PORTB&=~(1<<7);
			tempi=ReadADC(1);
			tempu8=(((520-tempi)*85)/(520))+150;
			OCR0B= tempu8;
			
		}
		else{
			OCR0B=0;
		}
		if (ReadADC(2)>530)
		{
			PORTB&=~(1<<0);
			PORTD|=(1<<7);
			tempi=ReadADC(2);
			tempu8=(((tempi-530)*85)/(1023-530))+130;
			OCR0A= tempu8;
			
		}
		else if (ReadADC(2)<520)
		{
			PORTB|=(1<<0);
			PORTD&=~(1<<7);
			tempi=ReadADC(2);
			tempu8=(((520-tempi)*85)/(520))+130;
			OCR0A= tempu8;
			
		}
		else{
			OCR0A=0;
		}
		if (((PINC&0b1)>>0)==0)
		{PORTB|=(1<<3);
		} 
		else
		{PORTB&=~(1<<3);
		}
		
	if ((PIND&0b1)==0)
	{	OCR0A=0;OCR0B=0;tempi=0;tempu8=0;
		longjmp (env, 1);
	
	}
		
	}

}

void automode()
{	LcdCommand(LCD_CLEARDISPLAY);
	LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKON);
	LcdSetCursor(1,0,"Length   count");
	LcdSetCursor(1,1,"00.0cm   00");
	LcdSetCursor(1,1,"");
	_delay_ms(1000);
	uint8_t pos=0;
	uint8_t lcdpos=0;
	uint8_t prevnum=0;
	char adcRead0 [1];
	

	while(((PINC&0b1)>>0)!=0){
	uint8_t adcread=ReadADC(3)/102.4;
	
	if (adcread!=prevnum)
	{	prevnum=adcread;
		switch(pos){
			case 0: lcdpos=1; lcddataRead[pos]=prevnum; break;
			case 1: lcdpos=2; lcddataRead[pos]=prevnum; break;
			case 2: lcdpos=4; lcddataRead[pos]=prevnum; break;
			case 3: lcdpos=10; lcddataRead[pos]=prevnum; break;
			case 4: lcdpos=11; lcddataRead[pos]=prevnum; break;
		}
		
		
		
		LcdSetCursor(lcdpos,1,itoa(adcread,adcRead0,10));
		LcdSetCursor(lcdpos,1,"");
		_delay_ms(100);
		
	}
		
		
	
	
	if (ReadADC(1)<50)
	{
		if (pos==0)
		{pos=0;
		}
		else{
		pos--;
		}
		switch(pos){
			case 0: lcdpos=1; lcddataRead[pos]=prevnum; break;
			case 1: lcdpos=2; lcddataRead[pos]=prevnum; break;
			case 2: lcdpos=4; lcddataRead[pos]=prevnum; break;
			case 3: lcdpos=10; lcddataRead[pos]=prevnum; break;
			case 4: lcdpos=11; lcddataRead[pos]=prevnum; break;
		}
		LcdSetCursor(lcdpos,1,itoa(adcread,adcRead0,10));
		LcdSetCursor(lcdpos,1,"");
		_delay_ms(500);
	}
	
	if (ReadADC(1)>1000)
	{
		if (pos==4)
		{pos=4;
		}
		else{
			pos++;
		}
		switch(pos){
			case 0: lcdpos=1; lcddataRead[pos]=prevnum; break;
			case 1: lcdpos=2; lcddataRead[pos]=prevnum; break;
			case 2: lcdpos=4; lcddataRead[pos]=prevnum; break;
			case 3: lcdpos=10; lcddataRead[pos]=prevnum; break;
			case 4: lcdpos=11; lcddataRead[pos]=prevnum; break;
		}
		LcdSetCursor(lcdpos,1,itoa(adcread,adcRead0,10));
		LcdSetCursor(lcdpos,1,"");
		_delay_ms(500);
	}
	
	
	
	}
	float templcd=lcddataRead[2];
	count=((lcddataRead[3])*10)+(lcddataRead[4]);
	length=(lcddataRead[0]*10)+(lcddataRead[1]);
	length+=templcd/10;
	
	LcdCommand(LCD_DISPLAYCONTROL|(LCD_DISPLAYON&~LCD_BLINKON));
	LcdCommand(LCD_CLEARDISPLAY);
	char chacter[16];
	sprintf(chacter,"length %4.1fcm ",length);
	LcdSetCursor(0,0,chacter);
	sprintf(chacter,"count  %d",count);
	LcdSetCursor(0,1,chacter);
	
}


int main(void)
{	setjmp (env);
	_delay_ms(10);
	sei();
	InitADC();
	PORTB&=~(1<<3);
	USART_Init(9600);
	xaxisInit();
	axisControlInit();
	yaxisInit();
	LcdInit();
	LcdSetCursor(4,0,"Welcome");
	_delay_ms(500);
	go_x(10,-6000);
	
	while(xEnable);
	_delay_ms(10);
	LcdCommand(LCD_CLEARDISPLAY);
	if (Modeselect()==0)// auto
	{	
		automode();
		
	} 
	else
	{
		ManualMode();
	}	
	
	
	for(uint8_t i=0;i<count;i++){
		go_y(1,0.20,30,length*yrate);
		while(yEnable);
		_delay_ms(10);
		go_x(50,1500);
		while(xEnable);
		_delay_ms(10);
		PORTB|=(1<<3);
		_delay_ms(500);
		go_x(50,3000);
		while(xEnable);
		_delay_ms(10);
		PORTB&=~(1<<3);
		_delay_ms(500);
		go_x(50,-2500);
		while(xEnable);
		_delay_ms(10);
		PORTB|=(1<<3);
		_delay_ms(500);
		go_x(50,-2000);
		while(xEnable);
		_delay_ms(10);
		PORTB&=~(1<<3);
		_delay_ms(500);
		if ((PIND&0b1)==0)
		{longjmp (env, 1);
		}
		
		
	}
	
	
    while (1) 
    {
		if ((PIND&0b1)==0)
		{longjmp (env, 1);
		}
		
		
    }
}

