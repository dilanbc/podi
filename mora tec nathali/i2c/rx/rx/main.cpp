/*
 * rx.cpp
 *
 * Created: 4/12/2022 2:10:37 AM
 * Author : dilan
 */ 

// Program for Slave mode
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

void init_slave(void);
uint8_t read_data(void);


unsigned char recv_data;

int main(void)
{

	DDRA=0xFF;
	init_slave();
	while(1)
	{
		
		//slave_listen();
		read_data();
	}
}

void init_slave(void)
{
	TWAR=0x20;
}



uint8_t read_data(void)
{
	TWCR=(1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	recv_data=TWDR;
	PORTA=recv_data;
	return TWDR;
}