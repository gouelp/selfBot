/*
 * selfBotControl.cpp
 *
 * Created: 02/06/2020 09:30:21
 * Author : Yaya
 */ 

/*
Main used to work on the moving base
*/

#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <stdio.h>

#include "dc_control.h"

int main(void){
/*    DDRB = 0x0F;
    PORTB = 0x09;
    DDRD |= (1 << 6) | (1<<5);
	TCNT0 = 0;
	TCCR0A = 0xA3;
	TCCR0B = 0x04;
	OCR0A = 0x67;
	OCR0B = 0x67;
*/
	init_PWM();
    while (1);
}

