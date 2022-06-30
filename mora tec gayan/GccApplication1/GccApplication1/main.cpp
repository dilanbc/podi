//datasheet  https://www.sparkfun.com/datasheets/Sensors/Temperature/MLX90614_rev001.pdf

#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include "LCD.h"
#include "I2C.h"

#define MLX90614Adress  0xb4
int erc = 0; // Variable holding the PEC value
int dataH = 0; // The second byte of data
int dataL = 0; // The first byte of data
double tempnalsb = 0.02; // Variable by which the digital value will be multiplied
double temperature = 0; // Variable holding the temperature

char  lcddata[20];

int main(void)
{
    LcdInit();
	TWIInit();
	
	
	
	
	_delay_ms(1000);
	
	
    while (1) 
    {
		TWIStart();//start twi
		TWIWriteAddrs(MLX90614Adress,0);//device address
		TWIWriteData(0x07);//register num  tempurature
		TWIStart();
		TWIWriteAddrs(MLX90614Adress,1);
		dataL=TWIReadACK();//read high bit
		dataH=TWIReadACK();//read low bit
		erc=TWIReadNACK();// Read the third (unimportant) data byte
		TWIStop();//end transmission
		
		
		
		
		
		temperature = (double) (((dataH & 0x007F) << 8) + dataL); // Create a 16-bit variable consisting of two one-byte variables
		temperature = temperature * tempnalsb; // For one bit 0.02 K, the result of this operation is the temperature in Kelvin
		
		temperature = temperature - 273.15; // Conversion to Celsius degrees
		
		sprintf(lcddata,"temp-%uC",temperature);
		LcdSetCursor(0,0,lcddata);
		
		
		
		TWIStart();//start i2c commnucation
		TWIWriteAddrs(MLX90614Adress,0);//device adress
		TWIWriteData(0x06);//Ambient temurature register 
		TWIStart();
		TWIWriteAddrs(MLX90614Adress,1);
		dataL=TWIReadACK();//read high data bits
		dataH=TWIReadACK();//read low data bits
		erc=TWIReadNACK();
		TWIStop();//end transmission
		
		temperature = (double) (((dataH & 0x007F) << 8) + dataL);
		temperature = temperature * tempnalsb;
		temperature = temperature - 273.15;
		
		sprintf(lcddata,"Ambi temp-%uC",temperature);//Ambient tempurature
		LcdSetCursor(0,2,lcddata);
			
		_delay_ms(200);
		
		
		
	}
		
		
		
	
}

