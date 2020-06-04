#include "dc_control.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

void init_PWM(){
/*
	This function initialize the PINs and the Timer used to control the 2 DC motors
	@Param : None
	@Return : None
*/
	DDRB |= (1 << DIR_L_1) | (1<<DIR_R_1) | (1<<DIR_L_2);
	DDRD |= (1 << MOTOR_L) | (1<<MOTOR_R) | (1<<DIR_R_2);
	TCNT0 = 0;	
	TCCR0A = 0xA3;
	TCCR0B = 0x05;
}

char set_direction(char dir){
/*
	This function set the directions of the 2 DC motors according to the direction the robot must take
	There are 2 pins by motor, one must be at HIGH level and the other at LOW, if they are at the same level the motor stop
	The motor direction depends on which one is at one on which one is at low
	@Param :	- dir : The direction the robot must take
	@Return : 1 if the order was understood and the direction set, -1 if there is a problem
*/
// If the order is forward the 2 motors turn in opposite directions to make wheels go in the same direction, and the robot goes forward
	if(dir == FORWARD){
		PORTB |= (1 << DIR_R_1) | (1 << DIR_L_2);
		PORTB &= ~(1 << DIR_L_1);
		PORTD &= ~(1 << DIR_R_2);
	}
// If the order is backward the 2 motors turn in opposite directions to make wheels go in the same direction, and the robot goes backward 
	else if(dir == BACKWARD){
		PORTB |= (1 << DIR_L_1);
		PORTB &= ~(1 << DIR_R_1) & ~(1 << DIR_L_2);
		PORTD |= (1 << DIR_R_2);
	}
/// If the order is static right, the motors turn in the same direction to make wheel turn in opposite direction and the robot turn on itself (left)
	else if(dir == STATIC_LEFT){
		PORTB |= (1 << DIR_L_2);
		PORTB &= ~(1 << DIR_L_1) & ~(1 << DIR_R_1);
		PORTD |= (1 << DIR_R_2);
	}
// If the order is static right, the motors turn in the same direction to make wheel turn in opposite direction and the robot turn on itself (right)
	else if(dir == STATIC_RIGHT){
		PORTB |= (1 << DIR_R_1) | (1 << DIR_L_1);
		PORTB &= ~(1 << DIR_L_2);
		PORTD &= ~(1 << DIR_R_2);
	}
// If it's not a known order, return -1 as error
	else{
		return -1;
	}
	return 1;
}

void change_PWM(char left_motor,char right_motor){
/*
	Change the rotation speed of the motors to adjust the speed of the robot
	Use a buffer for the conversion to be sure there is no overflow
	@Param :	- left_motor : The rotation speed of the left motor, from 0% to 100% capacity
				- right_motor : The rotation speed of the right motor, from 0% to 100 capacity
	@Return : None
*/
	int speedBuff = 0;
	speedBuff = (int)((left_motor*255)/100);
	OCR0A = (char)speedBuff;
	speedBuff = (int)((right_motor*255)/100);
	OCR0B = (char)speedBuff;
}

void stop_robot(){
/*
	This function stop the 2 DC motors to stop the robot
	The function simply send 0 to the ratios of the control PWM
	This is a shorter (and faster) version of change_PWM() because there is no conversion nor overflow check
	@Param : None
	@Return : None
*/
	OCR0A = 0x00;
	OCR0B = 0x00;
}