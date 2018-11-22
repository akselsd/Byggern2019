#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <stdint.h>
#include "can/CAN_driver.h"
#include "motor_box_driver.h"

typedef struct controller_values_struct
{
	uint8_t reference;
	float current_position;
	float error_sum;
	float old_error;
	float input;
	uint8_t ext_input_ctrl;
	motor_input output;
	
	float kp;
	float ki;
	float kd;
} controller_values;

void controller_init(uint8_t ext_input_ctrl);
void controller_clear(void);
void controller_set_reference(uint8_t ref);
void controller_set_input(float input);

#endif
