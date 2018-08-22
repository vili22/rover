#include "driverlib/fpu.h"

#include "serial.h"
#include "sensors.h"

#define input1 GPIO_PIN_5
#define input2 GPIO_PIN_4


float seconds_since_init;
//static unsigned int left_high = 0;
//static unsigned int right_high = 0;

float left_prev = -1.0f;
float right_prev = -1.0f;

struct EncoderState left_encoder;
struct EncoderState right_encoder;

static void init_peripherals(){
  //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
}

static void init_inputs(){

  //GPIODirModeSet(GPIO_PORTE_BASE, input1, GPIO_DIR_MODE_IN);
  //GPIOPadConfigSet(GPIO_PORTE_BASE, input1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  //GPIOIntTypeSet(GPIO_PORTE_BASE, input1, GPIO_RISING_EDGE);

  GPIODirModeSet(GPIO_PORTC_BASE, input1 | input2, GPIO_DIR_MODE_IN);
  GPIOPadConfigSet(GPIO_PORTC_BASE, input1 | input2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  GPIOIntTypeSet(GPIO_PORTC_BASE, input1 | input2, GPIO_BOTH_EDGES);
}

static void init_timer() {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_32_BIT_PER);
	TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet() - 1);
}
static void init_interrupts(){
  
  //GPIOPinIntEnable(GPIO_PORTE_BASE, input1);
  GPIOPinIntEnable(GPIO_PORTC_BASE, input1 | input2);
  IntEnable(INT_GPIOE);
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

static void init_encoder_states() {

	left_encoder.wheel_dir = 0;
	left_encoder.state_updated = 0;
	//left_encoder.time_stamp = -1;

	right_encoder.wheel_dir = 0;
	right_encoder.state_updated = 0;
	//right_encoder.time_stamp = -1;
}

static void set_left_encoder_state() {

//	float currentTime = sensors_get_seconds_since_init();
//
//	if(currentTime - left_prev < 0.01) {
//		return;
//	}
//
//	left_prev = currentTime;
	char rover_direction = motor_get_direction();
	unsigned int wheel_dir = 1;
	if(rover_direction == 'b' || rover_direction == 'l') {
		wheel_dir = 0;
	}
	left_encoder.wheel_dir = wheel_dir;
	left_encoder.state_updated = 1;
}

static void set_right_encoder_state() {

//	float currentTime = sensors_get_seconds_since_init();
//
//	if(currentTime - right_prev < 0.01) {
//		return;
//	}
//
//	right_prev = currentTime;
	char rover_direction = motor_get_direction();
	unsigned int wheel_dir = 1;
	if(rover_direction == 'b' || rover_direction == 'r') {
		wheel_dir = 0;
	}
	right_encoder.wheel_dir = wheel_dir;
	right_encoder.state_updated = 1;
}

void system_time() {
  	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
  	seconds_since_init = seconds_since_init + 1;
}

void IntGPIOe(void){

//	if(!GPIOPinRead(GPIO_PORTD_BASE, input1)){
//			return;
//	}
	char rover_direction = motor_get_direction();

	int wheel_dir = 1;
	if(rover_direction == 'b' || rover_direction == 'l') {
		wheel_dir = 0;
	}

	left_encoder.wheel_dir = wheel_dir;
	left_encoder.state_updated = 1;
	GPIOPinIntClear(GPIO_PORTE_BASE, input1);
}

void IntGPIOc(void){

	unsigned int source = GPIOPinIntStatus(GPIO_PORTC_BASE, input1 | input2);

	if(source == input1) {

		set_left_encoder_state();
		GPIOPinIntClear(GPIO_PORTC_BASE, input1);

	} else if (source == input2) {

		set_right_encoder_state();
		GPIOPinIntClear(GPIO_PORTC_BASE, input2);
	} else if( source == (input2 | input1)) {
		set_left_encoder_state();
		set_right_encoder_state();
		GPIOPinIntClear(GPIO_PORTC_BASE, input1 | input1);
	}
}

float sensors_get_seconds_since_init() {

	float seconds_since_init_copy = seconds_since_init;
	return (seconds_since_init_copy + (float)(SysCtlClockGet() -1 - TimerValueGet(TIMER2_BASE, TIMER_A)) / ((float) SysCtlClockGet()));
}

unsigned int sensors_left_encoder_updated() {

	return left_encoder.state_updated;
}

unsigned int sensors_right_encoder_updated() {

	return right_encoder.state_updated;
}

struct EncoderState sensors_get_left_encoder_state() {

	return left_encoder;
}

struct EncoderState sensors_get_right_encoder_state() {

	return right_encoder;
}

void sensors_clear_left_encoder_updated() {

	left_encoder.state_updated = 0;
}

void sensors_clear_right_encoder_updated() {

	right_encoder.state_updated = 0;
}

void init_sensors(void){

	init_fpu();
	init_peripherals();
	init_inputs();
	init_timer();
	init_interrupts();
	init_encoder_states();
}
    
