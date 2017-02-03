#include "driverlib/fpu.h"

#include "serial.h"
#include "sensors.h"

#define input1 GPIO_PIN_2
#define input2 GPIO_PIN_4


float seconds_since_init;
static float left_wheel_updated = -1.0f;
static float right_wheel_updated = -1.0f;
static float min_interval = 0.01f;

struct EncoderState encoder_state;

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

static void init_encoder_state() {

	encoder_state.wheel = 0;
	encoder_state.wheel_dir = 0;
	encoder_state.state_updated = 0;
}

void system_time() {
  	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
  	seconds_since_init = seconds_since_init + 1;
}

void IntGPIOd(void){

	float current_time = sensors_get_seconds_since_init();
	if(current_time - left_wheel_updated < min_interval) {
		return;
	}

	left_wheel_updated = current_time;
	char rover_direction = motor_get_direction();

	int wheel_dir = 1;
	if(rover_direction == 'b' || rover_direction == 'l') {
		wheel_dir = 0;
	}

	encoder_state.wheel = 0;
	encoder_state.wheel_dir = wheel_dir;
	encoder_state.state_updated = 1;
	GPIOPinIntClear(GPIO_PORTD_BASE, input1);
}

void IntGPIOc(void){

	float current_time = sensors_get_seconds_since_init();
	if(current_time - right_wheel_updated < min_interval) {
		return;
	}

	right_wheel_updated = current_time;

	char rover_direction = motor_get_direction();

	unsigned int wheel_dir = 1;
	if(rover_direction == 'b' || rover_direction == 'r') {
		wheel_dir = 0;
	}

	encoder_state.wheel = 1;
	encoder_state.wheel_dir = wheel_dir;
	encoder_state.state_updated = 1;

	GPIOPinIntClear(GPIO_PORTC_BASE, input2);
}

float sensors_get_seconds_since_init() {

	float seconds_since_init_copy = seconds_since_init;
	return (seconds_since_init_copy + (float)(SysCtlClockGet() -1 - TimerValueGet(TIMER2_BASE, TIMER_A)) / ((float) SysCtlClockGet()));
}

unsigned int sensors_encoder_updated() {

	return encoder_state.state_updated;
}

struct EncoderState sensors_get_encoder_state() {

	return encoder_state;
}

void sensors_clear_encoder_updated() {

	encoder_state.state_updated = 0;
}


void init_sensors(void){

	init_fpu();
	init_peripherals();
	init_inputs();
	init_timer();
	init_interrupts();
	init_encoder_state();
}
    
