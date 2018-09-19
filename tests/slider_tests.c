#include <stdio.h>
#include "../usb_multifunction_card/slider.h"
#include "tests.h"

void test_slider_position(void)
{
   slider_status status = slider_get_status();
   printf("Left: %u, Right: %u", status.left, status.right);
}
