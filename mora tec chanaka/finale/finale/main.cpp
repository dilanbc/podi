#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "LCD.h"

#define Buz_Port PORTD
#define Buz_DDR DDRD
#define Buzzer 7

const uint8_t kaypad[4][4]={{7,8,9,10},{4,5,6,11},{1,2,3,12},{13,0,14,15}};
uint8_t getkey();
uint8_t tempkey=0;
char lcddata[20];

uint8_t foodtype=0; // 0 vegi 1 fruit
#define vegi_select 7
#define fruit_select 8

uint8_t mashinh_level_cycles=0;
#define noMash_key 0
#define lowMash_key 1
#define highMash_key 2
#define noMash_cycles 0
#define lowMash_cycles 3
#define highMash_cycles 6

uint8_t quantity_time=0;
#define fill_Volume 100
#define quantity0_key 1
#define quantity1_key 2
#define quantity2_key 3
#define quantity0_time 20 //level 1 
#define quantity1_time 40 //level 2
#define quantity2_time 60 //level 3


uint16_t ultraINT=0;
volatile uint16_t TimerCal=0;// variable for collect echo data
#define foodlenght 50

#define stepDDR DDRD
#define stepPORT PORTD
#define DIR_CUT 0
#define STEP_CUT 1
#define cutCycles 1000

#define DIR_MASH 2
#define STEP_MASH 3
#define mashLength 1000



uint16_t flow;
uint16_t Reading=0; //analog Reading variable
uint16_t flowlimit=200;

void enterFoodtype();
void mashingLevel();
void enterQuantity();
void foodcut();
void boiling();
uint16_t ReadADC(uint8_t ADCchannel);
void mashing();

int main(void)
{
    LcdInit();
	uint8_t gl;
	
	
	
	while(1){//starting
		if (getkey()==13)
		{break;
		}
	}
	//welcome/////////////////////////////////////////////////////////////
	LcdSetCursor(4,0,"Welcome");
	_delay_ms(1000);
	LcdCommand(LCD_CLEARDISPLAY);
	
	
	
	enterFoodtype();
	mashingLevel();
	
	
	
	if (!foodtype)
	{enterQuantity();
	}
	
	foodcut();
	
	if (!foodtype)
	{boiling();
	}
	mashing();
    while (1) 
    {
		
		
		
    }
}

uint8_t getkey(){
	DDRC=0b00001111;
	PORTC=0b11110000;
	uint8_t getx;
	uint8_t gety;
	while(1){
		
	uint8_t pin=(~(PINC|0x0F));	
	if (pin)
	{	
		
	
		switch(pin){
		
		
		case (1<<4) :getx=0; break;
		case (1<<5) :getx=1;break;
		case (1<<6) :getx=2;break;
		case (1<<7) :getx=3;break;
		//DDRC=0b00001111;
		
		} //switch
		DDRC=0b0;
		PORTC=0b0;
		
		DDRC=0b11110000;
		PORTC=0b00001111;
		
		pin=~(PINC|0xF0);
		/*DDRD=0xFF;
		PORTD=pin;	*/
		
		
		switch(pin){
			case (1<<0) :gety=0;break;
			case (1<<1) :gety=1;break;
			case (1<<2) :gety=2;break;
			case (1<<3) :gety=3;break;
			
		} //switch
		
		DDRC=0b00001111;
		PORTC=0b11110000;
		
		
		return kaypad[gety][getx];
		
	}//if
	
	
	
	}//while 1
}

void mashingLevel(){
	//enter mashing level***************************************************
	LcdSetCursor(2,0,"Enter Mashing Level");
	LcdSetCursor(4,1,"Level");
	_delay_ms(1000);
	LcdCommand(LCD_CLEARDISPLAY);
	
	if (!foodtype)//vegi
	{
		sprintf(lcddata,"No mashing - %u",noMash_key);
		LcdSetCursor(0,0,lcddata);
		sprintf(lcddata,"Low-%u  High-%u",lowMash_key,highMash_key);
		LcdSetCursor(0,1,lcddata);
		
		while(1){
			
			 tempkey=getkey();
			if (tempkey==noMash_key)
			{
				LcdCommand(LCD_CLEARDISPLAY);
				LcdSetCursor(0,0,"No Mashing");
				mashinh_level_cycles=noMash_cycles;
				break;
			}
			else if(tempkey==lowMash_key)
			{
				LcdCommand(LCD_CLEARDISPLAY);
				LcdSetCursor(0,0,"Low Mashing");
				mashinh_level_cycles=lowMash_cycles;
				break;
			}
			
			else if(tempkey==highMash_key){
				LcdCommand(LCD_CLEARDISPLAY);
				LcdSetCursor(0,0,"High Mashing");
				mashinh_level_cycles=highMash_cycles;
				break;
			}
			
		}//end of while
		
	}
	else//fruit
	{
		sprintf(lcddata,"Low Mashing -%u",lowMash_key);
		LcdSetCursor(0,0,lcddata);
		sprintf(lcddata,"High Mashing-%u",highMash_key);
		LcdSetCursor(0,1,lcddata);
		
		while(1){
			 tempkey=getkey();
			
			if(tempkey==lowMash_key)
			{
				LcdCommand(LCD_CLEARDISPLAY);
				LcdSetCursor(0,0,"Low Mashing");
				mashinh_level_cycles=lowMash_cycles;
				break;
			}
			
			else if(tempkey==highMash_key){
				LcdCommand(LCD_CLEARDISPLAY);
				LcdSetCursor(0,0,"High Mashing");
				mashinh_level_cycles=highMash_cycles;
				break;
			}
			
		}// end of while
		
	}
	
	_delay_ms(1000);
	LcdCommand(LCD_CLEARDISPLAY);
	// end of mashing level///////////////////////////////////////////////////
}

void enterFoodtype(){
	LcdSetCursor(0,0,"Enter Food type");
	_delay_ms(2000);
	LcdCommand(LCD_CLEARDISPLAY);
	
	//enter food******************************************************
	sprintf(lcddata,"Food & vegi - %u",vegi_select);
	LcdSetCursor(0,0,lcddata);
	sprintf(lcddata,"Fruit       - %u",fruit_select);
	LcdSetCursor(0,1,lcddata);
	
	while(1){
		if (getkey()==vegi_select)
		{	LcdCommand(LCD_CLEARDISPLAY);
			LcdSetCursor(0,0,"vegetables");
			foodtype=0;
			break;
		}
		else if (getkey()==fruit_select)
		{foodtype=1;
			LcdCommand(LCD_CLEARDISPLAY);
			LcdSetCursor(0,0,"Fruits");
			break;
		}
	}
	_delay_ms(1000);
	LcdCommand(LCD_CLEARDISPLAY);
	// enter food type end/////////////////////////////////////////////////
}

void enterQuantity()
{
//enter quantity******************************************************
LcdSetCursor(2,0,"Enter Food ");
LcdSetCursor(0,1,"Quantity(grams)");
_delay_ms(2000);
LcdCommand(LCD_CLEARDISPLAY);

sprintf(lcddata,"40-50>%u 90-100>%u",quantity0_key,quantity1_key);
LcdSetCursor(0,0,lcddata);
sprintf(lcddata,"140-150>%u",quantity2_key);
LcdSetCursor(0,1,lcddata);

while(1){
	tempkey=getkey();
	if (tempkey==quantity0_key)
	{	LcdCommand(LCD_CLEARDISPLAY);
		LcdSetCursor(0,0,"Q- 40g-50g");
		quantity_time=quantity0_time;
		break;
	}
	else if (tempkey==quantity1_key)
	{
		LcdCommand(LCD_CLEARDISPLAY);
		LcdSetCursor(0,0,"Q- 90g-100g");
		quantity_time=quantity1_time;
		break;
	}
	
	else if (tempkey==quantity2_key)
	{
		LcdCommand(LCD_CLEARDISPLAY);
		LcdSetCursor(0,0,"Q- 140g-150g");
		quantity_time=quantity2_time;
		break;
	}
}
_delay_ms(1000);
LcdCommand(LCD_CLEARDISPLAY);
// quantity/////////////////////////////////////////////////
}

void foodcut(){
DDRA|=(1<<3); //D2 as output
TCCR0|=(1<<WGM01);//Enable Compare match mode
TCCR0|=(1<<CS11);//Start timer  prescaler =8
TCNT0=0;
OCR0=10;
TIMSK|=(1<<OCIE0);//enable timer Compare inturrept
sei();
Buz_DDR|=(1<<7);
Buz_Port|=(1<<7);

while (1)
{
	PORTA&=~(1<<3);//TRIG pin low
	_delay_us(50);//wait 50 micro sec
	PORTA|=(1<<3);//TRIG pin high
	_delay_us(50);//wait 50 micro sec
	PORTA&=~(1<<3);////TRIG pin low
	while(!(PINA&(1<<4)))//wait for pulse
	TimerCal=0;//rest timer
	while((PINA&(1<<4)))////wait for pulse down
	ultraINT=TimerCal;//copy timer
	uint8_t lengthcal=ultraINT/4.15;
	//sprintf(lcddata,"length %03u",ultraINT);
	//LcdSetCursor(0,0,lcddata);
	_delay_ms(200);
	
	if (lengthcal>foodlenght)
	{LcdSetCursor(2,0,"Please enter");
		LcdSetCursor(5,1,"foods");
	} 
	else
	{LcdCommand(LCD_CLEARDISPLAY);
		LcdSetCursor(0,0,"Food added");
		Buz_Port&=~(1<<7);
		_delay_ms(1000);
		LcdCommand(LCD_CLEARDISPLAY);
		break;
		
	}
	
}//while

stepDDR|=(1<<DIR_CUT)|(1<<STEP_CUT);
LcdSetCursor(0,0,"Cutting Process");

stepPORT|=(1<<DIR_CUT);

for(uint16_t i=0;i<cutCycles;i++){// cutting down
stepPORT|=(1<<STEP_CUT);
_delay_us(1000);
stepPORT&=~(1<<STEP_CUT);
_delay_us(1000);	
}
stepPORT&=~(1<<DIR_CUT);
for(uint16_t i=0;i<cutCycles;i++){//cutting up
	stepPORT|=(1<<STEP_CUT);
	_delay_us(1000);
	stepPORT&=~(1<<STEP_CUT);
	_delay_us(1000);
}
_delay_ms(1000);
LcdCommand(LCD_CLEARDISPLAY);


}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCal++;
	TCNT0=0;
	
}

void boiling(){
	LcdCommand(LCD_CLEARDISPLAY);
	DDRD|=(1<<4)|(1<<5);
	PORTD|=(1<<4);//water in
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));   // prescaler 128
	ADMUX |= (1<<REFS0)|(1<<REFS1);					//internal 2.56 v ref
	ADCSRA |= (1<<ADEN);                            // Turn on ADC
	//ADCSRA |= (1<<ADSC);                           // Do an initial conversion
	uint16_t timec=0;
	PORTA|=(1<<5);
	while(1){
		
		
		
		Reading=ReadADC(0)/10.23;
		sprintf(lcddata,"Flow %04u %04u",Reading,flow);
		LcdSetCursor(0,0,lcddata);
		
		flow+=Reading/10;
		
		Reading=(ReadADC(1)*0.25024438); //calibrated number
		sprintf(lcddata,"Temp %02u time %03u",Reading,timec);
		LcdSetCursor(0,1,lcddata);
		
	
		_delay_ms(1000);
		
		if (flow>fill_Volume)
		{PORTD&=~(1<<4);//water in off
		PORTD|=(1<<5);//heater on
			break;
		}
		
	}// solonoid
	
	
	while(1){
		Reading=ReadADC(0)/10.23;
		sprintf(lcddata,"Flow %04u %04u",Reading,flow);
		LcdSetCursor(0,0,lcddata);
		
		flow+=Reading/10;
		
		Reading=(ReadADC(1)*0.25024438); //calibrated number
		sprintf(lcddata,"Temp %02u time %03u",Reading,timec);
		LcdSetCursor(0,1,lcddata);
		_delay_ms(1000);
		
		if (PINA&(1<<5))
		{timec++;
			PORTD&=~(1<<4);//water in off
			PORTD|=(1<<5);//heater on
		} 
		else
		{PORTD|=(1<<4);//water in
			PORTD&=~(1<<5);//heater off
		}
		
		if (timec>quantity_time)
		{LcdCommand(LCD_CLEARDISPLAY);
			DDRA|=(1<<7);//water out
			PORTA|=(1<<7);//water out
			PORTD&=~(1<<5);//heater off
			LcdSetCursor(0,0,"Water Out");
			while(1){
				if (!(PINA&(1<<5)))
				{	
					break;
				}
			}
			_delay_ms(2000);
			PORTA&=~(1<<7);//water out off
			LcdSetCursor(0,0,"Heating down");
			_delay_ms(1000);
			
			while(1){
				Reading=(ReadADC(1)*0.25024438);
				if (Reading<60)
				{
					break;	
				}
			}
			DDRA|=(1<<2);
			PORTA|=(1<<2);
			LcdCommand(LCD_CLEARDISPLAY);
			LcdSetCursor(0,0,"Food Out");
			_delay_ms(3000);
			PORTA&=~(1<<2);
			LcdCommand(LCD_CLEARDISPLAY);
			break;
		}
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
	return ADCW;
}

void mashing(){
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(4,0,"Mashing");
	LcdSetCursor(4,1,"Process");
	stepDDR|=(1<<DIR_MASH)|(1<<STEP_MASH);
	
	Buz_Port|=(1<<Buzzer);
	PORTD|=(1<<6);//pull up
	while(1){
		if ((PIND&(1<<6)))
		{break;
		}
	}
	Buz_Port&=~(1<<Buzzer);
	
	
	for(uint8_t j=0;j<mashinh_level_cycles;j++){
	stepPORT|=(1<<DIR_MASH);	
	for(uint16_t i=0;i<cutCycles;i++){
		stepPORT|=(1<<STEP_MASH);
		_delay_us(1000);
		stepPORT&=~(1<<STEP_MASH);
		_delay_us(1000);
	}
	stepPORT&=~(1<<DIR_MASH);
	for(uint16_t i=0;i<cutCycles;i++){
		stepPORT|=(1<<STEP_MASH);
		_delay_us(1000);
		stepPORT&=~(1<<STEP_MASH);
		_delay_us(1000);
	}	
		
	}
	PORTD|=(1<<6);
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(0,0,"Open the door");
	_delay_ms(1000);
	Buz_DDR|=(1<<7);
	Buz_Port|=(1<<7);
	
	while(1){
		if (!(PIND&(1<<6)))
		{	Buz_Port&=~(1<<7);
			break;
		}
	}
	
	LcdCommand(LCD_CLEARDISPLAY);
	LcdSetCursor(2,0,"Thank you!");
	_delay_ms(1000);
	
	stepPORT|=(1<<DIR_MASH);	
	for(uint16_t i=0;i<cutCycles;i++){
		stepPORT|=(1<<STEP_MASH);
		_delay_us(1000);
		stepPORT&=~(1<<STEP_MASH);
		_delay_us(1000);
	}
	stepPORT&=~(1<<DIR_MASH);
	for(uint16_t i=0;i<cutCycles;i++){
		stepPORT|=(1<<STEP_MASH);
		_delay_us(1000);
		stepPORT&=~(1<<STEP_MASH);
		_delay_us(1000);
	}
	
}