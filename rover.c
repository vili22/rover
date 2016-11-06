#include "serial.h"
#include "sensors.h"

void init_clock(){
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
}

int main(void){

  //system clock initialization
  init_clock();
  init_board_leds();
  //serial initialization
  init_serial();
  //motor initialization
  init_motor();

  //sensor initialization
  init_sensors();

  //some general initialization
  init_master();



  while(1){

  }

}
