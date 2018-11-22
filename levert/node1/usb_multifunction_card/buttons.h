#ifndef USB_MULTIFUNCTION_BUTTONS_H
#define USB_MULTIFUNCTION_BUTTONS_H

#include <stdint.h>

typedef struct button_clicked
{
    uint8_t left;
    uint8_t right; 
} buttons_status;

/* Get current input status */
void usb_multifunction_buttons_get_status(buttons_status * status);

/* Init module */
void usb_multifunction_buttons_init(void);

#endif
