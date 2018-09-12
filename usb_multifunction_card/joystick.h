#ifndef INCLUDED_JOYSTICK_H
#define INCLUDED_JOYSTICK_H

typedef enum joystick_direction_enum
{
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3,
	NO_DIRECTION = 4
} joystick_direction;

typedef struct joystick_position
{
	unsigned char x;
	unsigned char y;
} joystick_status;

/* 
	Returns current joystick input
	@return x and y axis percentage (0-100). 0 is down/left 100 is up/right
*/
joystick_status joystick_get_status(void);
joystick_direction joystick_get_direction(const joystick_status * status);
void joystick_calibrate_joystick(void);

#endif