#ifndef F_CPU
#define F_CPU 1600000UL
#endif
#include <avr/io.h>
#include <util/delay.h>

#include "LCD/lcd.h"

int main()
{

	//PINC2 and PINC1 are declared as input pins
	DDRC &= ~(1<<PC2 | 1<<PC3);
	PORTC &= ~(1<<PC2 | 1<<PC3);
	int i;
	int count=50;
	char cnt[10];
	

	// initialize ADC and LCD
	lcd_init(LCD_DISP_ON_CURSOR); //CURSOR
	lcd_clrscr();
	lcd_gotoxy(0,0);
	_delay_ms(50);


	while(1)
	{    lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts("Counting... ");
		_delay_ms(200);
		
		if(PINC & (1<<PC2))    //Checks if the IR1 is detected
		{
				while(!(PINC & (1<<PC3)));    //waits for IR2 to detect
					while(PINC & (1<<PC3));    //Waits until, IR2 stops receiving the signal
					count--;
					lcd_clrscr();
					lcd_gotoxy(0,0);
					itoa(count,cnt,10);
					lcd_puts(cnt);
					lcd_gotoxy(3,0);
					lcd_puts("People can ");
					lcd_gotoxy(0,1);
					lcd_puts("enter the shop");
					_delay_ms(200);

			}

			
			if(PINC & (1<<PC3))    //Checks if the IR2 is detected
			{
				while(!(PINC & (1<<PC2)));  //waits for IR1 to detect
				while(PINC & (1<<PC2));     //Waits until, IR1 stops receiving the signal
				count++;
				lcd_clrscr();
				lcd_gotoxy(0,0);
				itoa(count,cnt,10);
				lcd_puts(cnt);
				lcd_gotoxy(2,0);
				lcd_puts(" people can");
				lcd_gotoxy(0,1);
				lcd_puts("enter the shop");
				_delay_ms(200);

		}
	}
	
	
}