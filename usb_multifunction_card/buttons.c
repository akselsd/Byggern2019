#include <avr/io.h>
#include "buttons.h"

#define READ_BIT(reg, bit) ((reg) & (bit))

buttons_status buttons_get_status(void)
{
    bool button_left = READ_BIT(PORTB, PIN0);
    bool button_right = READ_BIT(PORTB, PIN1);

    buttons_status status = {
	button_left;
	button_right;
    };
    return status;
}
