#include <stdio.h>
#include "tests.h"
#include "../usb_multifunction_card/buttons.h"

void button_test(void)
{
    buttons_status status = buttons_get_status();
    printf("button_left: %d, button_right: %d", status.left, status.right);
}
