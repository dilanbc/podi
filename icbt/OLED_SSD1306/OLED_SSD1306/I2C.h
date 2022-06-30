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


#endif /* I2C_H_ */