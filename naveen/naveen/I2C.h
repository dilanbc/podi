/*
 * I2C.h
 *
 * Created: 4/6/2021 9:41:47 PM
 *  Author: Dilan
 */ 


#ifndef I2C_H_
#define I2C_H_
#include <avr/io.h>
#include <util/delay.h>

#define SDA_PIN			PC4
#define SCL_PIN			PC5
#define I2C_PORT		PORTC

#define SCL_CLOCK		400000UL

void TWIInit(void);
uint8_t TWIStart(void);
uint8_t TWIStop(void);
uint8_t TWIWrite(uint8_t u8data);
uint8_t TWIReadACK(void);
uint8_t TWIReadNACK(void);


void TWIInit(){
	
	I2C_PORT |= (1 << SDA_PIN | 1 << SCL_PIN);			//Port Pullup
	TWCR = (1<<TWEN);
	TWSR = 0;
	TWBR = ((F_CPU/SCL_CLOCK)-16)/2;
	
}

uint8_t TWIStart(void)
{	uint8_t errorCount;
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT))){
		errorCount++;
		if (errorCount>100)
		{return 1;
		}
	}
	return 0;
}
//send stop signal
uint8_t TWIStop(void)
{	uint8_t errorCount;
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	while(TWCR&(1<<TWSTO)){
		errorCount++;
		if (errorCount>100)
		{return 1;
		}
	}
	return 0;
}

uint8_t TWIWrite(uint8_t u8data)
{
	TWDR = (u8data);
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
	return TWSR &(0xF8);
}


uint8_t TWIReadACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}
//read byte with NACK
uint8_t TWIReadNACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}
#endif /* I2C_H_ */