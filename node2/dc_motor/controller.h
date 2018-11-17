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
	motor_input output;
	
	float kp;
	float ki;
	float kd;
} controller_values;

void controller_init(void);
void controller_clear(void);
void controller_set_reference(CAN_message * msg);

#endif