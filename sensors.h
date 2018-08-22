/*
 * sensors.h
 *
 *  Created on: Oct 27, 2016
 *      Author: vvirkkal
 */
#ifndef SENSORS_H_
#define SENSORS_H_

struct EncoderState {
	//float time_stamp;
	unsigned int wheel_dir;
	unsigned int state_updated;
};
void init_sensors();
void IntGPIOe(void);
void IntGPIOc(void);
void system_time(void);
float sensors_get_seconds_since_init();
unsigned int sensors_left_encoder_updated();
unsigned int sensors_right_encoder_updated();
struct EncoderState sensors_get_left_encoder_state();
struct EncoderState sensors_get_right_encoder_state();
void sensors_clear_left_encoder_updated();
void sensors_clear_right_encoder_updated();


#endif /* SENSORS_H_ */
