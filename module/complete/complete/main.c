#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCDI2C.h"// lcd libary
#include "USART.h"

const uint8_t kaypad[4][4]={{7,8,9,10},{4,5,6,11},{1,2,3,12},{13,0,14,15}};
uint8_t getkeyNum();
char lcddata[20];
uint8_t keyout;


void EnterID();
void EnterPhone();
uint16_t PersonID=0;
uint16_t PhoneH=0;
uint16_t PhoneL=0;

uint8_t Pressure= 22;
uint8_t Tempurature=36;
#define peopleLimit 5

void ultralengthHand();
void ultralengthPeople();
volatile uint16_t TimerCalVal=0;// variable for collect echo data
uint16_t ultraPeople=0;//50
uint16_t ultraHand=0;//10

uint8_t PeopleCounter=0;

void servo(uint8_t pin);

uint16_t ReadADC(uint8_t ADCchannel);

int main(void)
{
	LcdInit(0x27);
	USART_Init(2400);
	
	TCCR0|=(1<<WGM01);//Enable Compare match mode
	TCCR0|=(1<<CS11);//Start timer  prescaler =8
	TCNT0=0;
	OCR0=10;
	TIMSK|=(1<<OCIE0);//enable timer Compare inturrept
	DDRC|=(1<<2)|(1<<5)|(1<<6);
	DDRD|=(1<<4);
	
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));   // prescaler 128
	ADMUX |= (1<<REFS0)|(1<<REFS1);                            // Intarnal 2.56 ref
	ADCSRA |= (1<<ADEN);                            // Turn on ADC
	ADCSRA |= (1<<ADSC);                            // Do an initial conversion
	DDRD|=(1<<5);
	
	MCUCR|=(1<<ISC11)|(1<<ISC10)|(1<<ISC01)|(1<<ISC00);
	GICR|=(1<<INT0)|(1<<INT1);
	
	LcdSetCursor(4,0,"Welcome");	
	
	
	
	
	while(1){
	ultralengthPeople();
	
		
	if (ultraPeople<50)
	{
		LcdCommand(LCD_CLEARDISPLAY);
		break;
	}
		_delay_ms(10);
	}
	
	sprintf(lcddata,"Count %02u",PeopleCounter);
	LcdSetCursor(0,0,lcddata);
	
	
	if (PeopleCounter<peopleLimit)
	{
		LcdSetCursor(0,1,"Limit OK");
	} 
	else
	{	
		LcdSetCursor(0,1,"People Limited");
		while(1){
			sprintf(lcddata,"Count %02u",PeopleCounter);
			LcdSetCursor(0,0,lcddata);
			
			if (PeopleCounter<peopleLimit)
			{
				LcdCommand(LCD_CLEARDISPLAY);
				break;
			}
			
		}
	}
	
	_delay_ms(2000);
	
	
	EnterID();
	EnterPhone();
	
	
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Place Hand");
	servo(1);
	
	while(1){
		ultralengthHand();
		if (ultraHand<10)
		{
			LcdCommand(LCD_CLEARDISPLAY);
			break;
		}
		_delay_ms(10);
	}
	
	
	
	uint16_t Readtemp=ReadADC(0)/4.05;
	sprintf(lcddata,"Tempura. %uC",Readtemp);
	LcdSetCursor(0,0,lcddata);
	
	if (Readtemp<37)
	{PORTD|=(1<<5);
	} 
	
		
	PORTC|=(1<<6);
	_delay_ms(1000);
	PORTC&=~(1<<6);
	_delay_ms(1000);
	servo(0);
	
	LcdCommand(LCD_CLEARDISPLAY);
	
	if (Readtemp>40)
	{
		
		USART_TxStringln("AT");
		_delay_ms(500);
		USART_TxStringln("AT");
		_delay_ms(500);
		USART_TxStringln("AT+CMGF=1");
		_delay_ms(500);
		USART_TxStringln("AT+CMGS=\"+94710000000\"");
		_delay_ms(200);

		USART_TxStringln("Warning");
		
		_delay_ms(200);
		USART_Transmit(26);
		_delay_ms(200);
		_delay_ms(1000);
		
		USART_TxStringln("AT");
		_delay_ms(500);
		USART_TxStringln("AT");
		_delay_ms(500);
		USART_TxStringln("AT+CMGF=1");
		_delay_ms(500);
		
		
		sprintf(lcddata,"AT+CMGS=\"+947%u%u\"",PhoneH,PhoneL);
		USART_TxStringln(lcddata);
	

		USART_TxStringln("You cannot enter");
		
		_delay_ms(200);
		USART_Transmit(26);
		_delay_ms(200);
		_delay_ms(1000);
		
		
		
		
	
		
		
	}
		

	
	
	
	
}

uint8_t getkeyNum(){
	DDRB=0b00001111;
	PORTB=0b11110000;
	uint8_t getx;
	uint8_t gety;
	while(1){
		
		uint8_t pin=(~(PINB|0x0F));
		if (pin)
		{
			
			
			switch(pin){
				
				
				case (1<<4) :getx=0; break;
				case (1<<5) :getx=1;break;
				case (1<<6) :getx=2;break;
				case (1<<7) :getx=3;break;
				
				
			} //switch
			DDRB=0b0;
			PORTB=0b0;
			
			DDRB=0b11110000;
			PORTB=0b00001111;
			
			pin=~(PINB|0xF0);
			
			switch(pin){
				case (1<<0) :gety=0;break;
				case (1<<1) :gety=1;break;
				case (1<<2) :gety=2;break;
				case (1<<3) :gety=3;break;
				
			} //switch
			
			DDRB=0b00001111;
			PORTB=0b11110000;
			
			
			return kaypad[gety][getx];
			
		}//if
		
		
		
	}//while 1
}


void EnterID(){
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Enter ID");
	_delay_ms(500);
	uint8_t pos=0;
	uint8_t numbers[4]={0,0,0,0};
	uint8_t posData[4][2]={{0,9},{1,9},{2,9},{3,9}};


	sprintf(lcddata,"%u%u%u%u",numbers[0],numbers[1],numbers[2],numbers[3]);
	LcdSetCursor(0,1,lcddata);
	LcdSetCursor(pos,1,"");
	LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKON);

	while(1){
		uint8_t keyout=getkeyNum();
		
		if (keyout<=posData[pos][1])
		{
			numbers[pos]=keyout;
			sprintf(lcddata,"%u%u%u%u",numbers[0],numbers[1],numbers[2],numbers[3]);
			LcdSetCursor(0,1,lcddata);
			pos++;
			if (pos>3)
			{pos=0;
			}
			LcdSetCursor(posData[pos][0],1,"");
			_delay_ms(200);
		}
		
		else if (keyout==14)
		{
			LcdCommand(LCD_CLEARDISPLAY);
			LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKOFF);
			LcdSetCursor(0,0,"ID Added");
			PersonID=(numbers[0]*1000)+(numbers[1]*100)+(numbers[2]*10)+numbers[3];
			_delay_ms(500);
			break;
		}
	}
}



void EnterPhone(){
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Enter PhoneNum.");
	_delay_ms(500);
	uint8_t pos=0;
	uint8_t numbers[8]={0,0,0,0,0,0,0,0};
	uint8_t posData[8][2]={{2,9},{3,9},{4,9},{5,9},{6,9},{7,9},{8,9},{9,9}};


	sprintf(lcddata,"07%u%u%u%u%u%u%u%u",numbers[0],numbers[1],numbers[2],numbers[3],numbers[4],numbers[5],numbers[6],numbers[7]);
	LcdSetCursor(0,1,lcddata);
	LcdSetCursor(pos,1,"");
	LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKON);
	LcdSetCursor(posData[pos][0],1,"");

	while(1){
		uint8_t keyout=getkeyNum();
		
		if (keyout<=posData[pos][1])
		{
			numbers[pos]=keyout;
			sprintf(lcddata,"07%u%u%u%u%u%u%u%u",numbers[0],numbers[1],numbers[2],numbers[3],numbers[4],numbers[5],numbers[6],numbers[7]);
			LcdSetCursor(0,1,lcddata);
			pos++;
			if (pos>7)
			{pos=0;
			}
			LcdSetCursor(posData[pos][0],1,"");
			_delay_ms(200);
		}
		
		else if (keyout==14)
		{
			LcdCommand(LCD_CLEARDISPLAY);
			LcdCommand(LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_BLINKOFF);
			LcdSetCursor(0,0,"Phone Num. Added");
			PhoneH=(numbers[0]*1000)+(numbers[1]*100)+(numbers[2]*10)+numbers[3];
			PhoneL=(numbers[4]*1000)+(numbers[5]*100)+(numbers[6]*10)+numbers[7];
			_delay_ms(500);
			break;
		}
	}
}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCalVal++;
	TCNT0=0;
	
}

void ultralengthPeople(){
	
	PORTC&=~(1<<2);//TRIG pin low
	_delay_us(50);//wait 50 micro sec
	PORTC|=(1<<2);//TRIG pin high
	_delay_us(50);//wait 50 micro sec
	PORTC&=~(1<<2);////TRIG pin low
	while(!(PINC&(1<<3)))//wait for pulse
	TimerCalVal=0;//rest timer
	while((PINC&(1<<3)))////wait for pulse down
	ultraPeople=TimerCalVal/4.1282;//copy timer value
}

void ultralengthHand(){
	
	PORTC&=~(1<<2);//TRIG pin low
	_delay_us(50);//wait 50 micro sec
	PORTC|=(1<<2);//TRIG pin high
	_delay_us(50);//wait 50 micro sec
	PORTC&=~(1<<2);////TRIG pin low
	while(!(PINC&(1<<4)))//wait for pulse
	TimerCalVal=0;//rest timer
	while((PINC&(1<<4)))////wait for pulse down
	ultraHand=TimerCalVal/4.1282;//copy timer value
}

void servo(uint8_t pin){
	if (pin)
	{
		for(uint8_t j=0;j<100;j++){
			PORTC|=(1<<5);
			for(uint8_t i=0;i<10;i++){
				_delay_us(100);
			}
			PORTC&=~(1<<5);
			
			
			for(uint8_t i=0;i<10;i++){
				_delay_us(100);
			}
		}
	} 
	else
	{
		
		for(uint8_t j=0;j<100;j++){
			
			PORTC|=(1<<5);
			for(uint8_t i=0;i<15;i++){
				_delay_us(100);
			}
			PORTC&=~(1<<5);
			
						
			for(uint8_t i=0;i<15;i++){
				_delay_us(100);
			}
		}
	}
}

ISR(INT0_vect){
		PeopleCounter++;
		PORTD^=(1<<4);
}
ISR(INT1_vect){
		if (PeopleCounter>0)
		{PeopleCounter--;
		}
		PORTD^=(1<<4);
}

uint16_t ReadADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	return ADCW;
}