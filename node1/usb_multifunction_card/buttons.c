#include <avr/io.h>
#include <stdio.h>
#include "buttons.h"
#include "bit_macros.h"
#include "can/CAN_driver.h"

static buttons_status current;

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


void usb_multifunction_buttons_transmit_status(void)
{
	usb_multifunction_buttons_get_status(&current);
	CAN_message * msg = CAN_message_constructor(ID_BUTTONS, 2);
	msg->data[0] = current.left;
	msg->data[1] = current.right;
	printf("Button: %d\n", current.right);
	CAN_send(msg);
	CAN_message_destructor(msg);
}