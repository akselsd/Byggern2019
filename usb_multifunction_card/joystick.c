#define F_CPU 4915200
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "joystick.h"
#include "usb_multifunction_card_io.h"

#define READ_BIT(reg, bit) ((reg) & (1 << bit))

#define MAX_JOYSTICK_VALUE 255
/* MAX_JOYSTICK_VALUE / 100 */
#define JOYSTICK_PERCENTAGE_FACTOR 1.28

/* x-axis */
#define CHANNEL_1 4
/* Y-axis */
#define CHANNEL_2 5

#define DEADZONE 5

/* Offset for joystick, initialized to zero for first read(calibration) */
joystick_status calibration_offset = {0, 0};

static joystick_direction coordinates_to_direction(char x, char y)
{
	if (x == 0 && y == 0)
    	return NO_DIRECTION;
	if (abs(x) < abs(y))
    	return y > 0 ? UP : DOWN;

    else if (abs(y) <= abs(x))
    	return x > 0 ? RIGHT : LEFT;

}

const char * joystick_direction_to_string(joystick_direction dir)
{
	switch(dir){
		case UP:
			return "UP";
		case DOWN:
			return "DOWN";
		case RIGHT:
			return "RIGHT";
		case LEFT:
			return "LEFT";
		case NO_DIRECTION:
			return "NO_DIRECTION";
		default:
			return "ERROR_DIRECTION";
	}
}

joystick_status joystick_get_status(void)
{
    volatile char * ADC = (volatile char *)0x1400;

	/* Read and offset 0*/
	bool pressed = READ_BIT(PINB, PB2);
	char x = read_channel(CHANNEL_1, ADC) - 128;
	char y = read_channel(CHANNEL_2, ADC) - 128;

	/* Convert to percentages */
	x = (float)x/JOYSTICK_PERCENTAGE_FACTOR;
	y = (float)y/JOYSTICK_PERCENTAGE_FACTOR;
	
	/* Offset from calibration */
	// x = x - calibration_offset.x;
	// y = y - calibration_offset.y;

	/* Check region for 0 output */
	x = abs(x) < DEADZONE ? 0 : x;
	y = abs(y) < DEADZONE ? 0 : y;

	/* Populate struct */
	joystick_status status = {
		pressed,
		x,
		y,
		coordinates_to_direction(x, y)
	};
	return status;
}



void joystick_calibrate_joystick(void)
{
	printf("--Calibrating joystick start--\n");
	_delay_ms(50);
	calibration_offset = joystick_get_status();
	printf("Result: X: %d, Y: %d\n",
		calibration_offset.x,
		calibration_offset.y);
	printf("--Calibrating joystick finish--\n");
}
