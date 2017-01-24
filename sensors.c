#include "driverlib/fpu.h"

#include "serial.h"
#include "string_utils.h"

float seconds_since_init;

#define input1 GPIO_PIN_2
#define input2 GPIO_PIN_4

static unsigned int left_high = 0;
static unsigned int right_high = 0;

static void init_peripherals(){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
}

static void init_inputs(){

  GPIODirModeSet(GPIO_PORTD_BASE, input1, GPIO_DIR_MODE_IN);
  GPIOPadConfigSet(GPIO_PORTD_BASE, input1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
  GPIOIntTypeSet(GPIO_PORTD_BASE, input1, GPIO_BOTH_EDGES);

  GPIODirModeSet(GPIO_PORTC_BASE, input2, GPIO_DIR_MODE_IN);
  GPIOPadConfigSet(GPIO_PORTC_BASE, input2, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
  GPIOIntTypeSet(GPIO_PORTC_BASE, input2, GPIO_BOTH_EDGES);
}

static void init_timer() {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_32_BIT_PER);
	TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet() - 1);
}
static void init_interrupts(){
  
  GPIOPinIntEnable(GPIO_PORTD_BASE, input1);
  GPIOPinIntEnable(GPIO_PORTC_BASE, input2);
  IntEnable(INT_GPIOD);
  IntEnable(INT_GPIOC);
  IntEnable(INT_TIMER2A);
  TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
  IntMasterEnable();
  TimerEnable(TIMER2_BASE, TIMER_A);

}

static void init_fpu() {

	FPULazyStackingEnable();
	FPUEnable();
	seconds_since_init = 0;
}

void system_time() {
  	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
  	seconds_since_init = seconds_since_init + 1;
}

void IntGPIOd(void){

	if(!GPIOPinRead(GPIO_PORTD_BASE, input1) && left_high){
		return;
	} else if(GPIOPinRead(GPIO_PORTD_BASE, input1) && !left_high) {
		return;
	}

	left_high = GPIOPinRead(GPIO_PORTD_BASE, input1) > 0 ? 0 : 1;
	int wheel = 0;
	int wheel_dir = 1;
	char rover_direction = motor_get_direction();

	if(rover_direction == 'f') {
		wheel_dir = 1;
	} else if(rover_direction == 'b') {
		wheel_dir = 0;
	} else if(rover_direction == 'l') {
			wheel_dir = 0;
	} else if(rover_direction == 'r') {
			wheel_dir = 1;
	}

	GPIOPinIntClear(GPIO_PORTD_BASE, input1);

	float realtime = seconds_since_init + (float)(SysCtlClockGet() -1 - TimerValueGet(TIMER2_BASE, TIMER_A)) / ((float) SysCtlClockGet());
	char message[80];
	int message_len = eb_sprintf(message, "%d %f %d %d\n", 0, realtime, wheel, wheel_dir);
	serial_write_message(message, message_len);
}

void IntGPIOc(void){

	if(!GPIOPinRead(GPIO_PORTC_BASE, input2) && right_high){
		return;
	} else if(GPIOPinRead(GPIO_PORTC_BASE, input2) && !right_high) {
		return;
	}

	right_high = GPIOPinRead(GPIO_PORTC_BASE, input2) > 0 ? 0 : 1;


	int wheel = 1;
	int wheel_dir = 1;
	char rover_direction = motor_get_direction();

	if(rover_direction == 'f') {
		wheel_dir = 1;
	} else if(rover_direction == 'b') {
		wheel_dir = 0;
	} else if(rover_direction == 'l') {
			wheel_dir = 1;
	} else if(rover_direction == 'r') {
			wheel_dir = 0;
	}

	GPIOPinIntClear(GPIO_PORTC_BASE, input2);

	float realtime = seconds_since_init + (float)(SysCtlClockGet() -1 - TimerValueGet(TIMER2_BASE, TIMER_A)) / ((float) SysCtlClockGet());
	char message[80];
	int message_len = eb_sprintf(message, "%d %f %d %d\n", 0, realtime, wheel, wheel_dir);
	serial_write_message(message, message_len);
}


void init_sensors(void){

	init_fpu();
	init_peripherals();
	init_inputs();
	init_timer();
	init_interrupts();
}
    
