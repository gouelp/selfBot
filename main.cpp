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

int main(void){
    DDRB = 0x0F;
    PORTB = 0x0A;
    DDRD |= (1 << 6) | (1<<5);
	TCNT0 = 0;
	TCCR0A = 0xA3;
	TCCR0B = 0x05;
	OCR0A = 0x00;
	OCR0B = 0x00;
    while (1) {
		
    }
}

