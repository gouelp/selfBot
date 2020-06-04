
#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <stdio.h>

#include "detection.h"

void init_servo(){
/*
	This function initialize the PIN and the Timer used to modify the angle of the servo motor
	@Param : None
	@Return : None
*/
	DDRB |= (1<<SERVO_PWM);
	TCCR1A |= (1 << WGM11) | (1 << COM1B1);
	TCCR1B |= (1 << WGM13) | (1 << WGM12) |(1 << CS11);
	ICR1 = 40000;
}

void turn_servo(unsigned char angle){ 
/*
	Set the servo motor according to the angle given in degree
	Max value = 4800 for 0 degree, min is 1050 for 180 degree
	The increment is about 21 per degree
	Look straight is about 2900
	@Param : angle : the angle required in degree
	@Return : None
*/
	OCR1B = 4800-(21*angle);
//	OCR1B = 1050+(21*angle);
}

void init_usSensor(){
/*
	This function initialize the PINs and the Timer used to measure distances with the ultrasonic sensor
	@Param : None
	@Return : None
*/
	DDRD |= (1<<US_TRIGGER);
	DDRD &= ~(1<<US_ECHO);
	TCCR2A = 0x00;
}

unsigned int calc_dist(){
/*
	This function measure the distance in centimeter using the ultrasonic sensor
	@Param : None
	@Return : The distance in centimeter, between 5cm and 4m
*/
	unsigned int ustime =0;
	long mesloop = 0;
// Set the Timer for 10us, start it and trigger the sensor
	TCNT2 = 0xEB;
	PORTD |= (1<<US_TRIGGER);
	TCCR2B = 0x02;
// Wait for 10us and stop the trigger and reset the Timer
	while((TIFR2&(1<<TOV2))==0); 
	PORTD &= ~(1<<US_TRIGGER);
	TCCR2B = 0x00;
	TIFR2 |= (1<<TOV2);
	TCNT2 = 0x00;
// Wait for the echo signal to start
	while((PIND&(1<<US_ECHO))==0);
// Start counting the time within the echo signal is HIGH
	TCCR2B = 0x03;
	while(((PIND&(1<<US_ECHO))!=0)){
		if((TIFR2&(1<<TOV2))!=0){
			mesloop++;
			TIFR2 |= (1<<TOV2);
		}
	}
// Stop counting
	TCCR2B = 0x00;
	TIFR2 |= (1<<TOV2);
// Convert the time in cm and return it
	ustime = ((255*mesloop)+TCNT2)*2;
	return ustime/58;
}


/*
  These functions are old versions of the function using the ultrasonic sensor with timer one.
  They are kept in case the ultrasonic sensor needs to be used without servo motor
*/
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