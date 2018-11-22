#ifndef INCLUDED_JOYSTICK_H
#define INCLUDED_JOYSTICK_H

#include <stdint.h>

typedef enum joystick_direction_enum
{
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3,
	NO_DIRECTION = 4
} joystick_direction;

typedef struct joystick_status_struct
{
	uint8_t pressed;
	uint8_t x;
	uint8_t y;
	joystick_direction dir;
} joystick_status;

/* Init module */
void usb_multifunction_joystick_init(void);

/* Get current joystick position */
void joystick_get_status(joystick_status * status);

/* Get string representation of direction */
const char * joystick_direction_to_string(joystick_direction dir);

#endif
