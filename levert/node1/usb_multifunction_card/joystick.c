#include "system_constants.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "bit_macros.h"
#include "joystick.h"
#include "usb_multifunction_card_io.h"

#define MAX_JOYSTICK_VALUE 255
/* MAX_JOYSTICK_VALUE / 100 */
#define JOYSTICK_PERCENTAGE_FACTOR 1.28
/* x-axis */
#define CHANNEL_1 4
/* Y-axis */
#define CHANNEL_2 5
#define DEADZONE 20
#define ADC 0x1400

void usb_multifunction_joystick_init(void)
{
	/* Set as input and enable pull-up */
	CLEAR_BIT(DDRB, PB2);
	SET_BIT(PORTB, PB2);
}

static joystick_direction coordinates_to_direction(uint8_t x, uint8_t y)
{
	int _x = x - 127;
	int _y = y - 127;
	if (_x == 0 && _y == 0)
    	return NO_DIRECTION;
	if (abs(_x) < abs(_y))
    	return _y > 0 ? UP : DOWN;

    else if (abs(_y) <= abs(_x))
    	return _x > 0 ? RIGHT : LEFT;

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

void joystick_get_status(joystick_status * status)
{
    volatile uint8_t * ADC_address = (volatile uint8_t *)ADC;

	/* Read and offset 0*/
	status->pressed = !READ_BIT(PINB, PB2);
	uint8_t x = read_channel(CHANNEL_1, ADC_address);
	uint8_t y = read_channel(CHANNEL_2, ADC_address);
	/* Check region for 0 output */
	x = ((x < DEADZONE + 128) &&  (x > 128 - DEADZONE)) ? 127 : x;
	y = ((y < DEADZONE + 128) &&  (y > 128 - DEADZONE)) ? 127 : y;
	
	
	status->x = x;
	status->y = y;
	status->dir = coordinates_to_direction(status->x, status->y);
}
