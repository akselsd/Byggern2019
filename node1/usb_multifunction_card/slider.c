#include "slider.h"
#include "usb_multifunction_card_io.h"
#include <stdio.h>

#define CHANNEL_3 6
#define CHANNEL_4 7
#define ADC 0x1400

static slider_status current; /* TODO: Move into transmit position? */

void slider_get_status(slider_status * status)
{
    volatile uint8_t * ADC_address = (volatile uint8_t *) ADC;

    status->left = read_channel(6, ADC_address);
    status->right = read_channel(7, ADC_address);

}


void slider_transmit_position(void)
{
    slider_get_status(&current);
	CAN_message * msg = CAN_message_constructor(ID_SLIDERS, 2);
	msg->data[0] = current.left;
	msg->data[1] = current.right;
	printf("LEFT %d -- RIGHT %d\n", current.left, current.right);
	CAN_send(msg);
	CAN_message_destructor(msg);

}