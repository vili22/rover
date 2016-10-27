#include "driverlib/fpu.h"

#include "serial.h"
#include "string_utils.h"

unsigned long seconds_since_init = 0;

#define input1 GPIO_PIN_2
#define input2  GPIO_PIN_6

static void init_peripherals(){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
}

static void init_inputs(){

  GPIODirModeSet(GPIO_PORTD_BASE, input1|input2, GPIO_DIR_MODE_IN);
  GPIOPadConfigSet(GPIO_PORTD_BASE, input1|input2, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
  GPIOIntTypeSet(GPIO_PORTD_BASE, input1 | input2, GPIO_BOTH_EDGES);
}

static void init_timer() {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_32_BIT_PER);
	TimerLoadSet(TIMER2_BASE, TIMER_A, SysCtlClockGet() - 1);
}
static void init_interrupts(){
  
  GPIOPinIntEnable(GPIO_PORTD_BASE, input1 | input2);
  IntEnable(INT_GPIOD);
  IntEnable(INT_TIMER2A);
  TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
  IntMasterEnable();
  TimerEnable(TIMER2_BASE, TIMER_A);

}

static void init_fpu() {

	FPULazyStackingEnable();
	FPUEnable();
}

void system_time() {
  	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
  	seconds_since_init = seconds_since_init + SysCtlClockGet();
}

void IntGPIOd(void){

	int wheel = 0;
	unsigned int source = GPIOPinIntStatus(GPIO_PORTD_BASE, input1 | input2);
	if(source == input1){
		wheel = 0;
	} else if(source == input2){
		wheel = 1;
	}

	GPIOPinIntClear(GPIO_PORTD_BASE, input1 | input2);

	unsigned long realtime = seconds_since_init + TimerValueGet(TIMER2_BASE, TIMER_A);
	char message[80];
	int message_len = eb_sprintf(message, "%d %l %d %d\n", 0, realtime, wheel, 1);
	write_message(message, message_len);
}

void init_sensors(void){

  init_peripherals();
  init_inputs();
  init_timer();
  init_interrupts();
  init_fpu();
}
    
