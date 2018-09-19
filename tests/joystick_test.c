#include <stdio.h>
#include "../usb_multifunction_card/joystick.h"

void test_joystick_position(void)
{
    joystick_status status = joystick_get_status();
    printf("X: %d, Y: %d, Direction: %s",
    	status.x,
    	status.y,
    	joystick_direction_to_string(status.dir));
}
