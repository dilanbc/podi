/*
 * spi.h
 *
 * Created: 12/29/2020 8:03:19 PM
 *  Author: Dilan
 */ 


#ifndef SPI_H_
#define SPI_H_

#define SPIPORT PORTB
#define SS1 4  //rc522
#define NRSTPD PINB1  //rc522
#define MISO 6
#define MOSI 5
#define SCK 7


void SPI_Init();
uint8_t SPI_Write(uint8_t incoming);

void SPI_Init( unsigned int ubrr)					/* SPI Initialize function */
{
	
	DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS1)|(1<<NRSTPD);	/* Make MOSI, SCK, SS 
						as Output pin */
	
	DDRB &= ~(1<<MISO);			/* Make MISO pin 
						as input pin */
	PORTB |= (1<<SS1);			/* Make high on SS pin */
	
	UCSRC = (1<<UMSEL1)|(1<<UMSEL0)|(0<<UCPHA)|(0<<UCPOL);
	UCSRB = (1<<RXEN)|(1<<TXEN);					
	//SPSR &= ~(1<<SPI2X);			/* Disable speed doubler */
	ubrr=(F_CPU/16/ubrr-1);  //4mhz,9600,+3
	/*Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
}

uint8_t SPI_Write(uint8_t incoming){
	
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = incoming;
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) );
	/* Get and return received data from buffer */
	return UDR;
}





#endif /* SPI_H_ */