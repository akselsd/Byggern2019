#include "slider.h"
#include "usb_multifunction_card_io.h"
#include <stdio.h>

#define CHANNEL_3 6
#define CHANNEL_4 7


slider_status slider_get_status(void)
{
    volatile uint8_t * ADC = (volatile uint8_t *) 0x1400;

    unsigned uint8_t left = read_channel(6, ADC);
    unsigned uint8_t right = read_channel(7, ADC);

    slider_status status = {
        left,
        right
    };

    return status;
}


void slider_transmit_position(void)
{
	slider_status status = slider_get_status();

	CAN_message * msg = CAN_message_constructor(1, 2);
	msg->data[0] = status.left;
	msg->data[1] = status.right;
	printf("Right: %d\n", status.right);
	CAN_send(msg);
	CAN_message_destructor(msg);
}