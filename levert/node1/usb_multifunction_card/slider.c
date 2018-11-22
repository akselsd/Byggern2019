#include "slider.h"
#include "usb_multifunction_card_io.h"
#include <stdio.h>

#define CHANNEL_3 6
#define CHANNEL_4 7
#define ADC 0x1400

static slider_status current; 

void slider_get_status(slider_status * status)
{
    volatile uint8_t * ADC_address = (volatile uint8_t *) ADC;

    status->left = read_channel(6, ADC_address);
    status->right = read_channel(7, ADC_address);

}
