/*
 * selfBotControl.cpp
 *
 * Created: 02/06/2020 09:30:21
 * Last updated : 04/06/2020
 * Author : Pierre-Vincent Gouel
 * Description : 
 * This file is the main file of the code that as for purpose to control a small and cheap autonomous rover based on Arduino but programmed without libraries
 */ 

/*
Main used to work on the moving base
*/

#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <stdio.h>

#include "dc_control.h"
#include "detection.h"

#define SET_TIMER 0

char time_count = 0;

void init_interrup(char timer);

int main(void){
/*
	Main function of the robot project
*/
    init_servo();
	init_usSensor();
	init_PWM();
	init_interrup(SET_TIMER);
	
    while (1);
}

void init_interrup(char timer){
/*
	This function initialize the interrupt bit of the appropriate timer register, and enable the global interruption system
	@Param : None
	@Return : None
*/
	if(timer == 0){
		TIMSK0 |= (1<<TOIE0);
	}
	sei();
}

ISR (TIMER0_OVF_vect){
/*
	Interruption routine
*/

//Wait for 40 interrupts, because the maximum time of timer 0 is too short to give motor the time to correctly turn
	if(time_count<= 40){ 
		time_count++;
		return;
	}
	time_count = 0;
	unsigned dist = calc_dist();
	if(dist >= 1 && dist <= 10){
		turn_servo(180);
	}
	else if (dist < 25){
	 turn_servo(90);
	}
	else{
		turn_servo(0);
	}
}