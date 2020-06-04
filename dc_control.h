#ifndef DCCONTROL_H
#define DCCONTROL_H

#define MOTOR_L 6
#define MOTOR_R 5
#define DIR_L_1 1
#define DIR_L_2 3
#define DIR_R_1 0
#define DIR_R_2 7

#define FORWARD 0
#define BACKWARD 1
#define STATIC_LEFT 2
#define STATIC_RIGHT 3

void init_PWM();
char set_direction(char dir);
void change_PWM(char ratio);

#endif