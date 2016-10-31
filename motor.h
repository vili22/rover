/*
 * motor.h
 *
 *  Created on: Nov 17, 2013
 *      Author: vvirkkal
 */
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "general_functions.h"

struct Motor{
	char speed;
	char direction;
	unsigned long shut_down_period;
	int shut_down_time;
};

void init_motor();
void drive_motor(char direction);
void motor_timer_interrupt();
void motor_set_speed(char speed);
char motor_get_direction();

#endif

