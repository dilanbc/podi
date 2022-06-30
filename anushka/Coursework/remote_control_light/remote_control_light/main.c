#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

bool isLightOn = true;
int lightIntensity = 0;

int main(void){
	/* enable FALLING edge interrupt on INT0 pin*/
	EICRA = 0b00000010;			//set falling edge interrupt on INT0 pin
	EIMSK = 0b00000001;			//enable external interrupt on INT0
	sei();						//enable globe interrupts
	
	/* configure the PWM signal generator 
	
	         < --- PWM pedio -->
	  		  _ _ _ _           _ _ _ _          _ _             _ _ _ _
	|        |       |         |       |        |   |           |
	|        |       |         |       |        |   |           |
	|_ _ _ _ |       |_ _ _ _ _|       | _ _ _ _|   |_ _ _ _ _ _|
	         <------>                           <--->  
	       PWM Duty cycle					PWM duty cycle
		   
	PWM period is defined by the PWM mode and TOP value (refer data sheet page 128)
	PWM duty cycle is define in OCR1A  registers
	*/
	DDRB = 0b00100000;			//make PORTB PD7 output
	TCCR1A = 0b10101001;		// set OC1A pin to clear automatically the TCNT1 value equals to OCR1A
								//fast PWM mode 8bit
	TCCR1B = 0b00001001;		//prescale value 1:1	
	
	/* infinite loop */
    while (1) {
		OCR1A = lightIntensity;
		_delay_ms(100);
    }
}

/*this is the interrupt function
  This function executed on every time someone generate falling signal on
  PD0 -- INT 0
  
  below is the falling edge
 
  _ _ _			
	   |		5V
	   |
	   ?		falling edge
	   |
	   |_ _ _ _ 0V
  

 */
ISR(INT0_vect){
	int code =  PINE;
	
	switch (PINE){
		case 0b00000111:					//if port reads as 111 ON or OFF the system based on previous condition
			if (isLightOn){					//if already on
				lightIntensity = 0;			//set light to full off
			}else{							//if already off
				lightIntensity = 255;		//set light to full ON	
			}
			isLightOn = !isLightOn;			//reset the status
			break;
		case 0b00000001:					//if port reads as 001 then increment the brightness
			if(isLightOn && lightIntensity + 50 <  255){
				lightIntensity += 50;
			}
			break;
		case 0b00000010:					//if port read as 010 decrement the brightness
			if(isLightOn  && lightIntensity - 50 >= 0){
				lightIntensity-= 50;
			}
	}
	
}
