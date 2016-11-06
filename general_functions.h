/*
 * general_functions.h
 *
 *  Created on: Nov 17, 2013
 *      Author: vvirkkal
 */
#ifndef _GENERAL_FUNCTIONS_H_
#define _GENERAL_FUNCTIONS_H_

#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"

void analogWrite(unsigned int port,int value);
void digitalWrite(int port,int value);
void init_board_leds();
void blink_led();
void init_master();

#endif
