
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "oled_ssd1306.h"

int main(void)
{
	
	oled_init();
	oled_gotoxy(0,2);
	oled_font_size(1);
	oled_write("22");
    while(1){}
       
    
}