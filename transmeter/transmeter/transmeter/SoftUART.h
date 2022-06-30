/*
 * SoftUART.h
 *
 * Created: 4/28/2020 2:56:31 PM
 *  Author: Dilan
 */ 


#ifndef SOFTUART_H_
#define SOFTUART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>


#define TX_PORT PORTD
#define TX_PIN  PD5
#define TX_DDR  DDRD
#define TX_DDR_PIN DDD5
char Softnum[]="";
volatile uint16_t tx_shift_reg = 0;

void SoftUART_Init();
void SoftUART_tx(char character);
void SoftUSART_tx_str(char* string);
void SoftUSART_tx_strln(char* string);
void SoftUSART_TxNumber(int32_t number,uint8_t dtype);

void SoftUART_Init(){
	//set TX pin as output
   TX_DDR |= (1<<TX_DDR_PIN);
   TX_PORT |= (1<<TX_PIN);
   //set timer0 to CTC mode
   TCCR2|= (1<<WGM21);
   //enable output compare 0 A interrupt
   TIMSK |= (1<<OCIE2);
   //set compare value to 103 to achieve a 9600 baud rate (i.e. 104µs)
   //together with the 8MHz/8=1MHz timer0 clock
   /*NOTE: since the internal 8MHz oscillator is not very accurate, this value can be tuned
     to achieve the desired baud rate, so if it doesn't work with the nominal value (103), try
     increasing or decreasing the value by 1 or 2 */
   OCR2 = 103;// 16mhz
   //enable interrupts
   sei();
}
void SoftUART_tx(char character)
{	 TX_PORT |= (1<<TX_PIN);//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	//if sending the previous character is not yet finished, return
	//transmission is finished when tx_shift_reg == 0
	if(tx_shift_reg){return;}
	//fill the TX shift register witch the character to be sent and the start & stop bits
	tx_shift_reg = (character<<1);
	tx_shift_reg &= ~(1<<0); //start bit
	tx_shift_reg |= (1<<9); //stop bit
	//start timer0 with a prescaler of 8
	TCCR2|= (1<<CS21);
	while(tx_shift_reg);
}

//timer0 compare A match interrupt
ISR(TIMER2_COMP_vect)
{
	//output LSB of the TX shift register at the TX pin
	if( tx_shift_reg & 0x01 )
	{
		TX_PORT |= (1<<TX_PIN); 
	}
	else
	{
		TX_PORT &=~ (1<<TX_PIN);
	}
	//shift the TX shift register one bit to the right
	tx_shift_reg = (tx_shift_reg >> 1);
	//if the stop bit has been sent, the shift register will be 0
	//and the transmission is completed, so we can stop & reset timer0
	if(!tx_shift_reg)
	{
		
		TCCR2&=~(0b111<<0);  //tomer stopped
		TCNT2 = 0;
	}
}

void SoftUSART_tx_str(char* string){

	while( *string ){
		SoftUART_tx( *string++ );
		//wait until transmission is finished
		
	}
}

void SoftUSART_TxNumber(int32_t number,uint8_t dtype=10){
	
	itoa(number,Softnum,dtype);
	SoftUSART_tx_str(Softnum);
	
}

void SoftUSART_tx_strln(char* st){
		 
	while( *st ){
		SoftUART_tx( *st++ );
		//wait until transmission is finished
		
	}
	SoftUART_tx(13);
	
	SoftUART_tx(10);
	
}


#endif /* SOFTUART_H_ */