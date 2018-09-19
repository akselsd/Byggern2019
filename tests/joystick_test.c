#include <stdio.h>
#include "../usb_multifunction_card/joystick.h"

void test_joystick_position(void)
{
    joystick_status status = joystick_get_status();
    printf("X: %u, Y: %u", status.x, status.y);
}
