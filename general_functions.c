#include "general_functions.h"

#define PWM1 GPIO_PIN_6
#define PWM2 GPIO_PIN_7

void init_board_leds(){

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

}

void init_master(){
	ROM_IntMasterEnable();
}

void analogWrite(unsigned int port,int value){


  unsigned long dutyCycle=49998-(49998*value)/255;
  if(port == PWM1)
    TimerMatchSet(TIMER0_BASE, TIMER_A,dutyCycle); // PWM
  else
    TimerMatchSet(TIMER0_BASE, TIMER_B,dutyCycle);

}

void digitalWrite(int port,int value){

  if(value == 1){
    if(port == 2)
      GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2,4);
    else
      GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3,8);
  }else
    if(port==2)
      GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2,0);
    else
      GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3,0);
}

void blink_led() {
	 if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1)) {
		 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
	 } else {
		 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
	 }
}
