#define F_CPU 4915200
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>

#include "joystick.h"
#include "usb_multifunction_card_io.h"

#define MAX_JOYSTICK_VALUE 255
/* MAX_JOYSTICK_VALUE / 100 */
#define JOYSTICK_PERCENTAGE_FACTOR 2.55 
#define MA0 0
#define MA1 1
#define MA2 2
#define MA3 3
/* x-axis */
#define CHANNEL_1 4
/* Y-axis */
#define CHANNEL_2 5

/* NOTE: Absolute value from ADC, NOT percentages */
joystick_status calibration_offset = {0, 0};

joystick_status joystick_get_status(void)
{
    volatile char * ADC = 0x1400;

	/* Read */
	unsigned char x = read_channel(CHANNEL_1, ADC);
	unsigned char y = read_channel(CHANNEL_2, ADC);


	/* Convert to percentages */
	joystick_status status = {
		(float)x/JOYSTICK_PERCENTAGE_FACTOR,
		(float)y/JOYSTICK_PERCENTAGE_FACTOR
	};
	return status;
}

joystick_direction joystick_status_to_direction(const joystick_status * status)
{
	if (status->x > 50)
    {
    }
}

void joystick_calibrate_joystick(void)
{
	printf("--Calibrating joystick start--\n");
	_delay_ms(50);
	calibration_offset = joystick_get_status();
	printf("Result: X: %u, Y: %u\n",
		calibration_offset.x,
		calibration_offset.y);
	printf("--Calibrating joystick finish--\n");
}
