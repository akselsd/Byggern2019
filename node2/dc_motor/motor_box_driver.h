#ifndef MOTOR_BOX_DRIVER_H_INCLUDED
#define MOTOR_BOX_DRIVER_H_INCLUDED

#include <stdint.h>

typedef enum motor_direction_enum
{
	MOTOR_LEFT,
	MOTOR_RIGHT
} motor_direction;

typedef struct motor_input_struct
{
	uint8_t speed;
	motor_direction dir;
} motor_input;

void motor_box_init(void);
int16_t motor_box_read(void);
void motor_box_set_direction(motor_direction dir);
void motor_box_set_speed(uint8_t speed);
void motor_box_reset_encoder(void);

#endif