#ifndef USB_MULTIFUNCTION_BUTTONS_H
#define USB_MULTIFUNCTION_BUTTONS_H

#include <stdbool.h>

typedef struct button_clicked
{
    bool left;
    bool right; 
} buttons_status;

buttons_status usb_multifunction_buttons_get_status(void);
void usb_multifunction_buttons_init(void);

#endif
