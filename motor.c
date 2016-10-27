#include "motor.h"

int PWM1=6,PWM2=7,DIR1=2,DIR2=3;
struct Motor motor;

static void init_motor_peripherals(){
   ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
}

static void init_motor_logic_pins(){

  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3);
}

static void init_motor_PWM(unsigned long ulPeriod, unsigned long dutyCycle){

  // Configure PB6 as T0CCP0
  ROM_GPIOPinConfigure(GPIO_PB6_T0CCP0);
  ROM_GPIOPinConfigure(GPIO_PB7_T0CCP1);
  ROM_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);
  ROM_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_7);
  
  // Configure timer
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM|TIMER_CFG_B_PWM);
  ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod -1);
  ROM_TimerLoadSet(TIMER0_BASE, TIMER_B, ulPeriod -1);
  ROM_TimerMatchSet(TIMER0_BASE, TIMER_A, dutyCycle); // PWM
  ROM_TimerMatchSet(TIMER0_BASE, TIMER_B,dutyCycle);
  ROM_TimerEnable(TIMER0_BASE, TIMER_A|TIMER_B);
}

static void init_motor_timer(){
	motor.shut_down_period = (ROM_SysCtlClockGet()/motor.shut_down_time);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);
	ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, motor.shut_down_period-1);
	ROM_IntEnable(INT_TIMER1A);
	ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	ROM_TimerEnable(TIMER1_BASE, TIMER_A);

}

static void init_motor_state() {

	motor.speed=122;
	motor.direction='s';
	motor.shut_down_time=3;
	motor.shut_down_period=(ROM_SysCtlClockGet()/motor.shut_down_time);

}

static void motor_keep_running(){

	ROM_TimerLoadSet(TIMER1_BASE, TIMER_A,motor.shut_down_period -1);
	ROM_TimerEnable(TIMER1_BASE, TIMER_A);
}

static void motor_advance() {

	analogWrite (PWM1,motor.speed);
	digitalWrite(DIR1,1);
	analogWrite (PWM2,motor.speed);
	digitalWrite(DIR2,1);
}

static void motor_reverse() {

  analogWrite (PWM1,motor.speed);
  digitalWrite(DIR1,0);
  analogWrite (PWM2,motor.speed);
  digitalWrite(DIR2,0);
}

static void motor_turn_left() {

  analogWrite (PWM1,motor.speed);
  digitalWrite(DIR1,1);
  analogWrite (PWM2,motor.speed);
  digitalWrite(DIR2,0);

}
static void motor_turn_right() {

  analogWrite (PWM1,motor.speed);
  digitalWrite(DIR1,0);
  analogWrite (PWM2,motor.speed);
  digitalWrite(DIR2,1);
}

static void motor_set_direction(char command) {

	motor.direction=command;
}

static void motor_stop() {

  analogWrite(PWM1,0);
  digitalWrite(DIR1,0);
  analogWrite (PWM2,0);
  digitalWrite(DIR2,0);
  motor_set_direction('s');
}

static char motor_get_direction() {

	return motor.direction;
}

void motor_set_speed(char speed){

	if(speed<0)
		motor.speed=0;
	else if(speed>255)
		motor.speed=255;
	else
		motor.speed=speed;

}

void motor_timer_interrupt(){
	// Clear the timer interrupt
	ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	motor_stop();
}

void init_motor(){

	init_motor_state();
	init_motor_peripherals();
	init_motor_logic_pins();
	init_motor_PWM(50000,49998);
	init_motor_timer();
}


void drive_motor(char direction){

		if(direction==motor_get_direction()){
			motor_keep_running();
			return;
		}

		if(direction=='f'){
			motor_keep_running();
			motor_advance();
			motor_set_direction(direction);
		}else if(direction=='b'){
			motor_keep_running();
			motor_reverse();
			motor_set_direction(direction);
		}
		else if(direction=='r'){
			motor_keep_running();
			motor_turn_right();
			motor_set_direction(direction);
		}
		else if(direction=='l'){
			motor_keep_running();
			motor_turn_left(200);
			motor_set_direction(direction);
		}
		else if(direction=='s'){
			motor_keep_running();
			motor_stop();
			motor_set_direction(direction);
		}
}




      

