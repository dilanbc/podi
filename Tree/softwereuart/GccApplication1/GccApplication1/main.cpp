#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "SoftUART.h"

int main(){
	SoftUART_Init();
	while(1){
	SoftUART_tx(65);
	_delay_ms(100);
	}
	
	
}
