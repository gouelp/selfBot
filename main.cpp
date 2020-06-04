/*
 * selfBotControl.cpp
 *
 * Created: 02/06/2020 09:30:21
 * Last updated : 04/06/2020
 * Author : Pierre-Vincent Gouel
 * Description : 
 * This file is the main file of the code that as for purpose to control a small and cheap autonomous rover based on Arduino but programmed without libraries
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <stdio.h>

#include "dc_control.h"
#include "detection.h"

#define SET_TIMER 0

char time_count = 0;
bool goal_achieved = false;

void init_interrup(char timer);
void navigate();
void test_system();
char init_robot();

int main(void){
/*
	Main function of the robot project
*/
	init_robot();
    while (1);
}

char init_robot(){
// Initialize all the part of the robot (registers)
	init_servo();
	init_usSensor();
	init_PWM();
// Do actions that must be done before starting the core program (interruption)
	turn_servo(90);
// Enable the interruptions
	init_interrup(SET_TIMER);
	
	return 0;	
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
	This function is called every time the interruptions system is enabled and the timer 0 overflows
	It's is the interruption routine, and not the infinite loop that contains the biggest part of the code
	This is done because it helps to keep better control of the time if others kind of interruptions are added
*/

//Waits for 40 interrupts, because the maximum time of timer 0 is too short to give motor the time to correctly turn
	if(time_count<= 40){ 
		time_count++;
		return;
	}
	time_count = 0;
//	test_system();
	navigate();
}

void navigate(){
	if(goal_achieved == true){return;}
	unsigned int dist = calc_dist();
	if(dist >= 1 && dist <= 10){
		stop_robot();
		turn_servo(0);	
		goal_achieved = true;
	}
	else{
		set_direction(FORWARD);
		change_PWM(40,40);	
	}
}

void test_system(){
/*
	Function used to test the implementations together
	Measure the distance of the nearest object and use the result to determine the action
	The action include modification of the Servo angle and a modification of the robot direction and speed
	Several configurations are tested here to check the system response
	@Param : None
	@Return : None
*/
	unsigned int dist = calc_dist();
	if(dist >= 1 && dist <= 8){
		turn_servo(180);
		stop_robot();
	}
	else if (dist <= 15){
		set_direction(STATIC_RIGHT);
		change_PWM(50,50);
	}
	else if (dist <= 25){
		turn_servo(90);
		set_direction(FORWARD);
		change_PWM(70,50);
	}
	else{
		turn_servo(0);
		set_direction(BACKWARD);
		change_PWM(50,50);
	}
}