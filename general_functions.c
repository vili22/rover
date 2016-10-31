#include "general_functions.h"

#define PWM1 GPIO_PIN_6
#define PWM2 GPIO_PIN_7

void init_board_leds(){

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

}

void init_master(){
	ROM_IntMasterEnable();
}

void analogWrite(unsigned int port,int value){


  unsigned long dutyCycle=49998-(49998*value)/255;
  if(port == PWM1)
    ROM_TimerMatchSet(TIMER0_BASE, TIMER_A,dutyCycle); // PWM
  else
    ROM_TimerMatchSet(TIMER0_BASE, TIMER_B,dutyCycle);

}

void digitalWrite(int port,int value){

  if(value == 1){
    if(port == 2)
      ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2,4);
    else
      ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3,8);
  }else
    if(port==2)
      ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2,0);
    else
      ROM_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3,0);
}
