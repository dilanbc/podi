/*
 * spi.h
 *
 * Created: 12/29/2020 8:03:19 PM
 *  Author: Dilan
 */ 


#ifndef SPI_H_
#define SPI_H_

#define SPIPORT PORTB
#define SS1 PINB2  //rc522
#define NRSTPD PINB1  //rc522
#define MISO PINB4
#define MOSI PINB3
#define SCK PINB5


void SPI_Init();
uint8_t SPI_Write(uint8_t incoming);

void SPI_Init()					/* SPI Initialize function */
{
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS1)|(1<<NRSTPD);	/* Make MOSI, SCK, SS 
						as Output pin */
	
	DDRB &= ~(1<<MISO);			/* Make MISO pin 
						as input pin */
	PORTB |= (1<<SS1);			/* Make high on SS pin */
	
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);	/* Enable SPI in master mode
						with Fosc/16 */
	//SPSR &= ~(1<<SPI2X);			/* Disable speed doubler */
	
}

uint8_t SPI_Write(uint8_t incoming){
	SPDR =incoming ;
	while ( !(SPSR & (1 << SPIF)) );
	return SPDR;
}





#endif /* SPI_H_ */