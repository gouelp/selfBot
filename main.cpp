/*
 * selfBotControl.cpp
 *
 * Created: 02/06/2020 09:30:21
 * Author : Yaya
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <stdio.h>

#include "detection.h"

#define SET_TIMER 0

char time_count = 0;

void init_interrup(char timer);

int main(void){
    init_servo();
	init_usSensor();
	TCNT0 = 0x00;
	TCCR0A = 0x00;
	TCCR0B = 0x05;
	
	init_interrup(SET_TIMER);
	
    while (1);
}

void init_interrup(char timer){
	if(timer == 0){
		TIMSK0 |= (1<<TOIE0);
	}
	sei();
}

ISR (TIMER0_OVF_vect){
	if(time_count<= 40){ //Wait for 40 interrupts, because the maximum time of timer 0 is too short to give motor the time to correctly turn
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