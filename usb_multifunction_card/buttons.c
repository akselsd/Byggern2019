#include <avr/io.h>
#include <stdio.h>
#include "buttons.h"

#define READ_BIT(reg, bit) ((reg) & (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))
#define SET_BIT(reg, bit) (reg |= (1 << bit))


void usb_multifunction_buttons_init(void)
{
	/* Configure pins to input */
	CLEAR_BIT(DDRB, PB1); /* Left */ 
	CLEAR_BIT(DDRB, PB0); /* Right */
}

buttons_status usb_multifunction_buttons_get_status(void)
{
	bool button_left = READ_BIT(PINB, PB1);
    bool button_right = READ_BIT(PINB, PB0);

    buttons_status status = {
		button_left,
		button_right,
    };
    return status;
}
