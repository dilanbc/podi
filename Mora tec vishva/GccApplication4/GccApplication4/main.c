#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

uint8_t hx711H=0; //Load Scale High Bits
uint16_t hx711L=0;//Load Scale Low Bits
float loadCellRead();
#define Load_data 2
#define Load_clk 3


float loadCellRead();
float hx=0;
void servoangle(uint8_t pin);

int main(void)
{
	DDRD|=(1<<Load_clk); //Load cell clock pin
	PORTD&=~(1<<Load_clk);//Clock pin low
	DDRD|=(1<<4);
	
	while (1)
	{
		hx=loadCellRead();
		if (hx>15)
		{servoangle(0);
		}
		else
		{servoangle(1);
		}
		_delay_ms(100);
	}
}


float loadCellRead(){
	hx711H=0;hx711L=0;  //clear variables
	for(uint8_t i=0;i<8;i++){  // Load cell data high 8 bits
		PORTD|=(1<<Load_clk); //Clock pin high
		_delay_us(10);
		if ((PIND&(1<<Load_data))>>Load_data)  //read data pin
		{hx711H|=(1<<(7-i));//set hx 711 varible
		}
		else
		{hx711H&=~(1<<(7-i));
		}
		PORTD&=~(1<<Load_clk); //Clock pin low
		_delay_us(5);
	}
	
	
	for(uint8_t i=0;i<16;i++){ // Load cell data low 16 bits
		PORTD|=(1<<Load_clk); //Clock pin high
		_delay_us(10);
		if ((PIND&(1<<Load_data))>>Load_data) //read data pin
		{hx711L|=(1<<(15-i));
		}
		else
		{hx711L&=~(1<<(15-i));
		}
		PORTD&=~(1<<Load_clk); //Clock pin low
		_delay_us(5);
	}
	
	hx711L=hx711L>>1; //shift bits
	
	if (hx711H&1)  //bit setup
	{hx711L|=(1<<15);
	}
	else
	{hx711L&=~(1<<15);
	}
	hx711H=hx711H>>1;
	
	return (hx711H*(65536/18029.6))+hx711L/18029.6; //load cell calibration
}

void servoangle(uint8_t pin){

	if (pin)
	{
		for(uint8_t j=0;j<100;j++){
			PORTD|=(1<<4);
			for(uint8_t i=0;i<10;i++){
				_delay_us(100);
			}
			PORTD&=~(1<<4);
			
			uint8_t ser=200-10;
			
			for(uint8_t i=0;i<ser;i++){
				_delay_us(100);
			}
		}
	}
	else
	{for(uint8_t j=0;j<100;j++){
		
		PORTD|=(1<<4);
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
		PORTD&=~(1<<4);
		
		uint8_t ser=200-15;
		
		for(uint8_t i=0;i<15;i++){
			_delay_us(100);
		}
	}
}




}