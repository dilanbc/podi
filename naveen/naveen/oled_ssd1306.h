/*
 *  Created: 19.06.2016 11:38:00
 *  Author: Ulrich
 */ 

#ifndef __oled_H
	#define __oled_H

	#include <inttypes.h>
	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <ctype.h>
	#include <string.h>
	#include <util/delay.h>

	#define SDA_PIN			PC4
	#define SCL_PIN			PC5
	#define I2C_PORT		PORTC

	#define OLED_I2C_ADDR	0x78
	#define SCL_CLOCK		400000UL

	void oled_init(void);
	void oled_clear_screen(void);
	void oled_font_size (uint8_t byte);
	void oled_gotoxy(uint8_t x, uint8_t y);
	void oled_write_char(char c);
	void oled_write_str(char* str);
	void oled_write_dot(uint8_t xd, uint8_t yd);
	void oled_write_P (const char *Buffer,...);

	#define oled_write(format, args...)   oled_write_P(PSTR(format) , ## args)

	#define COMMAND						0x00
	#define DATA						0x40

	#define SET_CONTRAST_CONTROL		0x81
	#define SET_DISPLAY_ON				0xAF
	#define SET_DISPLAY_OFF				0xAE
	#define SET_DISPLAY_NORMAL			0xA6
	#define SET_DISPLAY_INVERSE		0xA7
	#define ENTIRE_DISPLAY_ON			0xA4
	
	#define SET_MEMORY_ADDR_MODE		0x20
	#define HORIZONTAL_ADDRESSING_MODE	0x00
	#define VERTICAL_ADDRESSING_MODE	0x01
	#define PAGE_ADDRESSING_MODE		0x02
		
	#define SET_PAGE_START				0xB0	//~0xB7
	#define SET_COLUMN_ADDRESS			0x21
	#define SET_PAGE_ADDRESS			0x22
	
	#define SET_COM_OUTPUT_NORMAL		0xC0
	#define SET_COM_OUTPUT_REMAPPED		0xC8
	
	#define SET_DISPLAY_OFFSET			0xD3
	#define SET_DISPLAY_CLOCK			0xD5
	#define SET_PRECHARGE_PERIOD		0xD9
	#define SET_COM_PINS				0xDA
	
	#define SET_VCOMH_DESELECT			0xDB	//0x00,0x20,0x30
	
	#define	SET_LOWER_COLUMN			0x00
	#define	SET_HIGHER_COLUMN			0x10
	#define	SET_DISPLAY_START_LINE		0x40
	
	#define SET_SEGMENT_REMAP_O1		0xA0	
	#define SET_SEGMENT_REMAP_O2		0xA1
	
	#define SET_MULTIPLEX_RATIO			0xA8
	#define COMMAND_NO_OPERATION		0xE3
	
	#define CHARGE_BUMB_SETTING			0x8D



#include "font.h"
#include "I2C.h"

static uint8_t oled_x = 0, oled_y = 0, font_size = 0;

const uint8_t ssd1306_init_cmd [] PROGMEM = {
SET_DISPLAY_OFF,
COMMAND_NO_OPERATION,
SET_MEMORY_ADDR_MODE, HORIZONTAL_ADDRESSING_MODE,
SET_PAGE_START,
SET_COM_OUTPUT_REMAPPED, 
SET_LOWER_COLUMN,
SET_HIGHER_COLUMN,
SET_DISPLAY_START_LINE,			
ENTIRE_DISPLAY_ON, 
SET_SEGMENT_REMAP_O2,
SET_DISPLAY_NORMAL,	
SET_CONTRAST_CONTROL,	0xFF,	
SET_MULTIPLEX_RATIO,	0x3F,	
SET_DISPLAY_OFFSET,		0x00, 
SET_DISPLAY_CLOCK,		0xF0,
SET_PRECHARGE_PERIOD,	0x22, 
SET_COM_PINS,			0x12, 
SET_VCOMH_DESELECT,		0x20, 
CHARGE_BUMB_SETTING,	0x14, 
SET_DISPLAY_ON };

//***************************************************************************************


//***************************************************************************************


void oled_init (void){
	TWIInit();
	TWIStart();
	TWIWrite(OLED_I2C_ADDR);
	TWIWrite(COMMAND);
	for (uint8_t tmp = 0; tmp< sizeof(ssd1306_init_cmd); tmp++) {	
		TWIWrite(pgm_read_byte(&ssd1306_init_cmd[tmp]));
	}
	TWIStop();
    oled_clear_screen();
}

//***************************************************************************************
void oled_clear_screen (void){
	oled_gotoxy(0,0);
	TWIStart();
	TWIWrite(OLED_I2C_ADDR);
    TWIWrite(DATA);
    for (uint16_t i = 0; i < 128<<4 ; i++) {	// 128 * (64 / Byte) 
       TWIWrite(0);	
    }
    TWIStop();	
	oled_gotoxy(0,0);
}

//***************************************************************************************
void oled_gotoxy (uint8_t x, uint8_t y){
	oled_x = x;
	oled_y = y;
    TWIStart();
	TWIWrite(OLED_I2C_ADDR);
    TWIWrite(COMMAND);
    TWIWrite(SET_PAGE_START|y);
	TWIWrite(SET_COLUMN_ADDRESS);
    TWIWrite(x * 8);
	TWIWrite(SET_DISPLAY_START_LINE|0x3F);
    TWIStop();			
}

//***************************************************************************************
void oled_font_size (uint8_t byte){
	font_size = byte;
}

//***************************************************************************************
void oled_write_char (char c){
	if(font_size != 0){		
		uint8_t tmp = 0;
		oled_gotoxy(oled_x,oled_y);
		TWIStart();
		TWIWrite(OLED_I2C_ADDR);
		TWIWrite(DATA);
		for (uint8_t count = 0; count < 8; count++){
			tmp = pgm_read_byte(&font[(unsigned char)c][count]);
			tmp = tmp & 0x0f;
			tmp = ((tmp&0x01)*3)+(((tmp&0x02)<<1)*3)+(((tmp&0x04)<<2)*3)+(((tmp&0x08)<<3)*3);
			TWIWrite(tmp);
			TWIWrite(tmp);
		}
		TWIStop();	
		oled_gotoxy(oled_x,oled_y + 1);
		TWIStart();
		TWIWrite(OLED_I2C_ADDR);
		TWIWrite(DATA);
		for (uint8_t count = 0; count < 8; count++){
			tmp = pgm_read_byte(&font[(unsigned char)c][count]);
			tmp = (tmp & 0xf0)>>4;
			tmp = ((tmp&0x01)*3)+(((tmp&0x02)<<1)*3)+(((tmp&0x04)<<2)*3)+(((tmp&0x08)<<3)*3);
			TWIWrite(tmp);
			TWIWrite(tmp);
		}
		TWIStop();	
		oled_x +=2;
		oled_y -=1;
	}else{
		oled_gotoxy(oled_x,oled_y);
		TWIStart();
		TWIWrite(OLED_I2C_ADDR);
		TWIWrite(DATA);
		for (uint8_t count = 0; count < 8; count++){
			TWIWrite(pgm_read_byte(&font[(unsigned char)c][count]));
		}
		TWIStop();	
		oled_x +=1;
	}
}

//***************************************************************************************
void oled_write_str (char* str){
    while (*str) {
        oled_write_char(*str++);
    }
}

//***************************************************************************************
void oled_write_dot(uint8_t xd, uint8_t yd){
	
	TWIStart();
	TWIWrite(OLED_I2C_ADDR);
	TWIWrite(COMMAND);
	TWIWrite(SET_PAGE_START|(yd/8));
	TWIWrite(SET_COLUMN_ADDRESS);
	TWIWrite(xd);
	TWIWrite(SET_DISPLAY_START_LINE|0x3F);
	TWIStop();
	
	TWIStart();
	TWIWrite(OLED_I2C_ADDR);
	TWIWrite(DATA);
	
	TWIWrite(0b1<<(yd%8));
	
	TWIStop();
	
	
}
//***************************************************************************************
void oled_write_P (const char *Buffer,...)
{
	va_list ap;
	va_start (ap, Buffer);	
	
	int format_flag;
	char str_buffer[10];
	char str_null_buffer[10];
	char move = 0;
	char Base = 0;
	int tmp = 0;
	char by;
	char *ptr;
		
	//Ausgabe der Zeichen
    for(;;){
		by = pgm_read_byte(Buffer++);
		if(by==0) break; // end of format string
            
		if (by == '%'){
            by = pgm_read_byte(Buffer++);
			if (isdigit(by)>0){                   
 				str_null_buffer[0] = by;
				str_null_buffer[1] = '\0';
				move = atoi(str_null_buffer);
                by = pgm_read_byte(Buffer++);
				}

			switch (by){
                case 's':
                    ptr = va_arg(ap,char *);
                    while(*ptr) { oled_write_char(*ptr++); }
                    break;
				case 'b':
					Base = 2;
					goto ConversionLoop;
				case 'c':
					//Int to char
					format_flag = va_arg(ap,int);
					oled_write_char (format_flag++);
					break;
				case 'i':
					Base = 10;
					goto ConversionLoop;
				case 'o':
					Base = 8;
					goto ConversionLoop;
				case 'x':
					Base = 16;
					//****************************
					ConversionLoop:
					//****************************
					itoa(va_arg(ap,int),str_buffer,Base);
					int b=0;
					while (str_buffer[b++] != 0){};
					b--;
					if (b<move){
						move -=b;
						for (tmp = 0;tmp<move;tmp++){
							str_null_buffer[tmp] = '0';
							}
						//tmp ++;
						str_null_buffer[tmp] = '\0';
						strcat(str_null_buffer,str_buffer);
						strcpy(str_buffer,str_null_buffer);
						}
					oled_write_str (str_buffer);
					move =0;
					break;
				}
		}else{
			oled_write_char (by);	
		}
	}
	va_end(ap);
}
#endif // __oled_H 
