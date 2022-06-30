#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "I2C.h"
#include "USART.h"

void Readtime();
void setTime(uint8_t hSet,uint8_t mSet,uint8_t sSet );
void servoangle(uint8_t pin);
uint8_t hourC;
uint8_t minC;
uint8_t secC;
uint8_t checkS,R,G,B=0;
char lcddata[20];
volatile uint16_t TimerCal=0;// variable for collect echo data
uint16_t ultraINT0=0;
uint16_t ultraINT1=0;

uint8_t tabtimes[3][2]={{8,30},{12,15},{5,30}};

#define InputPIN PINA
#define Sensor 0
#define LDR 1

int main(void)
{	
	/*Ultrasonic Timer Part*/
	DDRD|=(1<<3); //D4 as output
	TCCR0|=(1<<WGM01);//Enable Compare match mode
	TCCR0|=(1<<CS11);//Start timer  prescaler =8
	TCNT0=0;
	OCR0=10;
	/*register value= time*(clock speed/prescale)
	register value=0.000001*(8000000/1)
	register value=10*/
	TIMSK|=(1<<OCIE0);//enable timer Compare inturrept
	sei();
	
	TWIInit();
	USART_Init(9600);
	DDRD|=(0b11<<5);
	DDRD|=(1<<4);
	DDRC|=(0b11111<<3);
   LcdInit();///////////////////////////////////////////////////////////////
   LcdSetCursor(4,0,"Welcome");////////////////////////////////////////
   _delay_ms(100);
   LcdCommand(LCD_CLEARDISPLAY);
   setTime(12,00,55);
    while (1) 
    {
		Readtime();
		
		PORTD&=~(1<<3);//TRIG pin low
		_delay_us(50);//wait 50 micro sec
		PORTD|=(1<<3);//TRIG pin high
		_delay_us(50);//wait 50 micro sec
		PORTD&=~(1<<3);////TRIG pin low
		while(!(PIND&(1<<2)))//wait for pulse
		TimerCal=0;//rest timer
		while((PIND&(1<<2)))////wait for pulse down
		ultraINT0=TimerCal/4.148148148148148;//copy timer value
		_delay_ms(10);
		PORTD&=~(1<<3);//TRIG pin low
		_delay_us(50);//wait 50 micro sec
		PORTD|=(1<<3);//TRIG pin high
		_delay_us(50);//wait 50 micro sec
		PORTD&=~(1<<3);////TRIG pin low
		while(!(PIND&(1<<7)))//wait for pulse
		TimerCal=0;//rest timer
		while((PIND&(1<<7)))////wait for pulse down
		ultraINT1=TimerCal/4.148148148148148;//copy timer value
		
		
		uint8_t sensorReading=(InputPIN&(1<<Sensor))>>Sensor;
		uint8_t LDRReading=(InputPIN&(1<<LDR))>>LDR;
		sprintf(lcddata,"%02u:%02u:%02u %01u %02u %01u",hourC,minC,secC,sensorReading,ultraINT0,LDRReading);  ///////////////////////////////////////
		LcdSetCursor(0,0,lcddata);///////////////////////////////////////////////////////////////////////////////////////////
		
		PORTD=(PORTD&0b10011111)|(0b1<<5);
		_delay_ms(50);
		R=rchar;
		
		PORTD=(PORTD&0b10011111)|(0b10<<5);
		_delay_ms(50);
		G=rchar;
		
		PORTD=(PORTD&0b10011111)|(0b11<<5);
		_delay_ms(50);
		B=rchar;
		PORTD=(PORTD&0b10011111);
		
		sprintf(lcddata,"%03u %03u %03u   %02u",R,G,B,ultraINT1);/////////////////////////////////////////////////////////
		LcdSetCursor(0,1,lcddata);//////////////////////////////////////////////////////////////////////////////////////
		
		if ((hourC==tabtimes[0][0])&&(minC==tabtimes[0][1])&&(secC<5))
		{servoangle(5);
		}
		
		if ((hourC==tabtimes[1][0])&&(minC==tabtimes[1][1])&&(secC<5))
		{servoangle(6);
		}
		if ((hourC==tabtimes[2][0])&&(minC==tabtimes[2][1])&&(secC<5))
		{servoangle(7);
		}
		
		if (LDRReading)
		{PORTC&=~(1<<4);
		}
		else{
			PORTC|=(1<<4);
		}
		if ((ultraINT0<20)||(ultraINT1<20))
		{
			PORTD|=(1<<4);
		} 
		else
		{PORTD&=~(1<<4);
		}
		
		if ((R==127)&&(G==127)&&(B==127))
		{PORTC|=(1<<3);
			_delay_ms(3000);
		} 
		else
		{PORTC&=~(1<<3);
		}
		
		if (sensorReading==1)
		{PORTC|=(1<<3);
			USART_TxStringln("AT");
			_delay_ms(500);
			USART_TxStringln("AT");
			_delay_ms(500);
			USART_TxStringln("AT+CMGF=1");
			_delay_ms(500);
			USART_TxStringln("AT+CMGS=\"+94710000000\"");
			_delay_ms(200);
			USART_TxString("Emergency");
			_delay_ms(200);
			USART_Transmit(26);
			_delay_ms(200);
			_delay_ms(30000);
		} 
		else
		{PORTC&=~(1<<3);
		}
		
		_delay_ms(100);
		
    }
}

ISR(TIMER0_COMP_vect){//ultrasonic
	TimerCal++;
	TCNT0=0;	
}


void Readtime(){
	uint8_t bit[5];
	TWIStart();/* Start I2C with device write address */
	TWIWriteAddrs(104,0); //Return 24 mean device found, return mean no device
	TWIWriteData(0);  //Register address
	
	TWIStart();
	TWIWriteAddrs(104,1); //Write bit
	bit[0]=TWIReadACK();
	bit[1]=TWIReadACK();
	bit[2]=TWIReadNACK();
	//bit[3]=TWIReadNACK();
	//h|=TWIReadACK()<<8;
	//h|=TWIReadNACK()<<16;
	TWIStop();
	secC=(((bit[0]&0xF0)>>4)*10)+(bit[0]&0xF);
	minC=(((bit[1]&0xF0)>>4)*10)+(bit[1]&0xF);
	hourC=(((bit[2]&0b00110000)>>4)*10)+(bit[2]&0xF);
	//dayC=bit[3]&0b111;
}

void setTime(uint8_t hSet,uint8_t mSet,uint8_t sSet){
	TWIStart();
	TWIWriteAddrs(104,0);
	TWIWriteData(0x02);
	TWIWriteData(((hSet/10)<<4)|(hSet%10));
	TWIStop();
	
	TWIStart();
	TWIWriteAddrs(104,0);
	TWIWriteData(0x01);
	TWIWriteData(((mSet/10)<<4)|(mSet%10));
	TWIStop();
	
	TWIStart();
	TWIWriteAddrs(104,0);
	TWIWriteData(0x00);
	TWIWriteData(((sSet/10)<<4)|(sSet%10));
	TWIStop();

	
}

void servoangle(uint8_t pin){
	
	for(uint8_t j=0;j<100;j++){
		PORTC|=(1<<pin);
		for(uint8_t i=0;i<10;i++){
			_delay_us(100);
		}
		PORTC&=~(1<<pin);
		
		uint8_t ser=200-10;
		
		for(uint8_t i=0;i<ser;i++){
			_delay_us(100);
		}
	}
	_delay_ms(2000);
	for(uint8_t j=0;j<100;j++){
		
		PORTC|=(1<<pin);
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
		PORTC&=~(1<<pin);
		
		uint8_t ser=200-15;
		
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
	}
	
	
	
	
}