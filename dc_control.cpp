#include "dc_control.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

void init_PWM(){
	char res;
	DDRB |= 0x0F;
	res = set_direction(FORWARD,0x67);
	DDRD |= (1 << MOTOR_L) | (1<<MOTOR_R);
	TCNT0 = 0;	
	TCCR0A = 0xA3;
	TCCR0B = 0x05;
}

char set_direction(char dir, char speed){
	if(dir == FORWARD){
		PORTB |= (1 << DIR_R_1) | (1 << DIR_L_2);
		PORTB &= ~(1 << DIR_L_1) & ~(1 << DIR_R_2);
	}
	else if(dir == BACKWARD){
		PORTB |= (1 << DIR_L_1) | (1 << DIR_R_2);
		PORTB &= ~(1 << DIR_R_1) & ~(1 << DIR_L_2);
	}
	else if(dir == LEFT){
		PORTB |= (1 << DIR_R_2) | (1 << DIR_L_2);
		PORTB &= ~(1 << DIR_L_1) & ~(1 << DIR_R_1);
	}
	else if(dir == RIGHT){
		PORTB |= (1 << DIR_R_1) | (1 << DIR_L_1);
		PORTB &= ~(1 << DIR_R_2) & ~(1 << DIR_L_2);
	}
	else{
		return -1;
	}
	change_PWM(speed);
	return speed;
}

void change_PWM(char ratio){
	OCR0A = ratio;
	OCR0B = ratio;
}