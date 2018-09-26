#include "tests.h"
#include "../usb_multifunction_card/buttons.h"
#include "../usb_multifunction_card/joystick.h"
#include "../usb_multifunction_card/slider.h"
#include <stdio.h>

void test_usb_multifunction_board(void)
{
    buttons_status bstatus = usb_multifunction_buttons_get_status();
   	slider_status sstatus = slider_get_status();
    joystick_status jstatus = joystick_get_status();
    printf("X: %d, Y: %d, Dir: %s, JB: %d, BR: %d, BL: %d, R: %u, L: %u\n",
    	jstatus.x,
    	jstatus.y,
    	joystick_direction_to_string(jstatus.dir),
    	jstatus.pressed,
    	bstatus.right,
    	bstatus.left,
    	sstatus.right,
    	sstatus.left);
}