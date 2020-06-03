#ifndef DEF_DETECT
#define DEF_DETECT

void init_usSensor();
void init_servo();
void turn_servo(char angle);
unsigned int calc_dist();
/* Older versions of the functions using the ultrasonic sensor */
unsigned int calc_dist_timer_1();
void init_usSensor_timer_1();

#endif