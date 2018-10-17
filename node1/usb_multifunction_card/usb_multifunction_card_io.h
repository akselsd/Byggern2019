#ifndef USB_MULTIFUNCTION_CARD_IO_H
#define USB_MULTIFUNCTION_CARD_IO_H

#include <stdint.h>

uint8_t read_channel(char channel_config, volatile uint8_t* unit);

#endif
