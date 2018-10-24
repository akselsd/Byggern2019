#define F_CPU 4915200

#include <util/delay.h>
#include "usb_multifunction_card_io.h"

// TODO change this to either be specific for ADC or more general in terms of timing
uint8_t read_channel(uint8_t channel_config, volatile uint8_t * unit)
{
	*unit = channel_config;
	_delay_us(50);
	return *unit;
}
