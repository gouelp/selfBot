#ifndef DEF_DETECT
#define DEF_DETECT

#define US_TRIGGER 4
#define US_ECHO 3
#define SERVO_PWM 2

/* Initialization functions */
void init_usSensor();
void init_servo();

/* Other functions */
void turn_servo(unsigned char angle);
unsigned int calc_dist();

/* Older versions of the functions using the ultrasonic sensor */
unsigned int calc_dist_timer_1();
void init_usSensor_timer_1();

#endif