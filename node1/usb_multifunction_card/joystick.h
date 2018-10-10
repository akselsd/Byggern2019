#ifndef INCLUDED_JOYSTICK_H
#define INCLUDED_JOYSTICK_H

#include <stdint.h>

void usb_multifunction_joystic_init(void);

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
	char x;
	char y;
	joystick_direction dir;
} joystick_status;

/* 
	Returns current joystick input
	@return x and y axis percentage (0-100). 0 is down/left 100 is up/right
*/
joystick_status joystick_get_status(void);
const char * joystick_direction_to_string(joystick_direction dir);
void joystick_calibrate_joystick(void);
void joystick_transmit_position(void);

#endif
