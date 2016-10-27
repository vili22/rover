#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "general_functions.h"
#include "driverlib/uart.h"
#include "motor.h"

#define MAX_MESSAGE_LEN 2

void init_serial();
void write_message(const char *message, int message_len);
void UARTIntHandler();

#endif
