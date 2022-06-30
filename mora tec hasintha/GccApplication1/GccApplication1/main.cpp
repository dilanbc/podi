#define F_CPU 8000000
#include <avr/io.h>
#include "LCD.h"

int main(void)
{
    LcdInit();
	LcdSetCursor(0,0,"Sample text");
    while (1) 
    {
    }
}

