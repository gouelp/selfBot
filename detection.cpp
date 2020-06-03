
#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <stdio.h>

#include "detection.h"

void init_servo(){
	DDRB |= (1<<2);
	TCCR1A |= (1 << WGM11) | (1 << COM1B1);
	TCCR1B |= (1 << WGM13) | (1 << WGM12) |(1 << CS12) |(1 << CS10);
	ICR1 = 312;
}

void turn_servo(char angle){ // !!!! NOT IN degree or rad just the ration for 1ms or 1.5ms or 2ms, conversion in angle todo
	OCR1B = angle;
}

void init_usSensor(){
	DDRB |= (1<<5);
	DDRB &= ~(1<<4);
	TCCR2A = 0x00;
}

unsigned int calc_dist(){
	unsigned int ustime =0;
	long mesloop = 0;
	TCNT2 = 0xEB;
	// Start Timer
	TCCR2B = 0x02;
	// Send HIGH
	PORTB |= (1<<5);
	while((TIFR2&(1<<TOV2))==0); // Wait for overflow
	TCCR2B = 0x00;
	TIFR2 |= (1<<TOV2);
	/* Stop timer and send LOW again */
	PORTB &= ~(1<<5);
	TCNT2 = 0x00;
	while((PINB&(1<<4))==0);
	TCCR2B = 0x03;
	while(((PINB&(1<<4))!=0)){
		if((TIFR2&(1<<TOV2))!=0){
			mesloop++;
			TIFR2 |= (1<<TOV2);
		}
	}
	TCCR2B = 0x00;
	TIFR2 |= (1<<TOV2);
	ustime = ((255*mesloop)+TCNT2)*2;
	TCCR2B = 0x00;
	return ustime/58;
}

void init_usSensor_timer_1(){
	DDRB |= (1<<5);
	DDRB &= ~(1<<4);
	TCCR1A = 0x00;
	TCCR1B = 0x02;
}

unsigned int calc_dist_timer_1(){
	unsigned int ustime =0;
	TCNT1 = 0xFFEB;
	// Start Timer
	TCCR1B = 0x02;
	// Send HIGH
	PORTB |= (1<<5);
	while((TIFR1&(1<<TOV1))==0); // Wait for overflow
	TCCR1B = 0x00;
	TIFR1 |= (1<<TOV1);
	/* Stop timer and send LOW again */
	PORTB &= ~(1<<5);
	TCNT1 = 0x0000;
	while((PINB&(1<<4))==0);
	TCCR1B = 0x03;
	antinoise:
	while(!((PINB&(1<<4))==0));
	if(!((PINB&(1<<4))==0)){goto antinoise;}
	ustime = TCNT1*4;
	TCCR1B = 0x00;
	return ustime/58;
}