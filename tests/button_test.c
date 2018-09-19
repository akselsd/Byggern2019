#include <stdio.h>
#include "tests.h"
#include "../usb_multifunction_card/buttons.h"

void test_buttons(void)
{
    buttons_status status = usb_multifunction_buttons_get_status();
    printf("button_left: %d, button_right: %d", status.left, status.right);
}
