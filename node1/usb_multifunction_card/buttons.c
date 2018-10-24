#include <avr/io.h>
#include <stdio.h>
#include "buttons.h"
#include "bit_macros.h"

void usb_multifunction_buttons_init(void)
{
	/* Configure pins to input */
	CLEAR_BIT(DDRB, PB1); /* Left */ 
	CLEAR_BIT(DDRB, PB0); /* Right */
}

void usb_multifunction_buttons_get_status(buttons_status * status)
{
	status->left = READ_BIT(PINB, PB1);
    status->right = READ_BIT(PINB, PB0);
}
