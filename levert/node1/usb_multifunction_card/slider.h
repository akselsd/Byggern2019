#ifndef SLIDER_H
#define SLIDER_H
#include "can/CAN_driver.h"

typedef struct slider_position
{
    uint8_t left;
    uint8_t right;
} slider_status;

/* Populate struct with current input */
void slider_get_status(slider_status * status);

#endif
