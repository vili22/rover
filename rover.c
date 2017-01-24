#include "serial.h"
#include "sensors.h"
#include "string_utils.h"

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

  //int LED = 2;
  while(1){

	  if(sensors_encoder_updated()) {
		  float realtime = sensors_get_seconds_since_init();
		  struct EncoderState state = sensors_get_encoder_state();
		  char message[80];
		  int message_len = eb_sprintf(message, "%d %f %d %d\n", 0, realtime, state.wheel, state.wheel_dir);
		  serial_write_message(message, message_len);
		  //sensors_clear_encoder_updated();
		  //new_time = sensors_get_seconds_since_init();
		  //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, LED);
		  // Delay for a bit
		  // Cycle through Red, Green and Blue LEDs
		  //if (LED == 8) {LED = 2;} else {LED = LED*2;}
		  sensors_clear_encoder_updated();
	  }
  }

}
