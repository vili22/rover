/*
 * sensors.h
 *
 *  Created on: Oct 27, 2016
 *      Author: vvirkkal
 */
#ifndef SENSORS_H_
#define SENSORS_H_

struct EncoderState {

	unsigned int wheel;
	unsigned int wheel_dir;
	unsigned int state_updated;
};
void init_sensors();
void IntGPIOd(void);
void IntGPIOc(void);
void system_time(void);
float sensors_get_seconds_since_init();
unsigned int sensors_encoder_updated();
struct EncoderState sensors_get_encoder_state();
void sensors_clear_encoder_updated();


#endif /* SENSORS_H_ */
