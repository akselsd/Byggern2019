#ifndef SLIDER_H
#define SLIDER_H
#include "can/CAN_driver.h"

typedef struct slider_position
{
    uint8_t left;
    uint8_t right;
} slider_status;

void slider_get_status(slider_status * status);
void slider_transmit_position(void);

#endif
