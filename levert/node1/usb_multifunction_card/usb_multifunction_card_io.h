#ifndef USB_MULTIFUNCTION_CARD_IO_H
#define USB_MULTIFUNCTION_CARD_IO_H

#include <stdint.h>

/* Read ADC channel */
uint8_t read_channel(uint8_t channel_config, volatile uint8_t * unit);

#endif
