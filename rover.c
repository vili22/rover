#include "serial.h"
#include "sensors.h"
#include "string_utils.h"

static void send_encoder_state(unsigned int wheel, unsigned int dir, float time_stamp) {

	 char message[80];
	 int message_len = eb_sprintf(message, "%d %f %d %d\n", 0, time_stamp, wheel, dir);
	 serial_write_message(message, message_len);
	 sensors_clear_left_encoder_updated();
}

static void init_clock(){
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

	  if(sensors_left_encoder_updated()) {
		  struct EncoderState state = sensors_get_left_encoder_state();
		  send_encoder_state(0, state.wheel_dir, sensors_get_seconds_since_init());
		  sensors_clear_left_encoder_updated();
	  }

	  if(sensors_right_encoder_updated()) {
		  struct EncoderState state = sensors_get_right_encoder_state();
		  send_encoder_state(1, state.wheel_dir, sensors_get_seconds_since_init());
		  sensors_clear_right_encoder_updated();
	  }
  }

}
