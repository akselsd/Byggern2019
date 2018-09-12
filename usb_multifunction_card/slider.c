#include <util/delay.h>
#include "slider.h"
#include "usb_multifunction_card_io.h"

#define CHANNEL_3 6
#define CHANNEL_4 7


slider_status slider_get_status(void)
{
    volatile char * ADC = 0x1400;

    unsigned char left = read_channel(6, ADC);
    unsigned char right = read_channel(7, ADC);

    slider_status status = {
        left,
        right
    };

    return status;
}
