#define F_CPU 4915200

#include <util/delay.h>
#include "usb_multifunction_card_io.h"

// TODO change this to either be specific for ADC or more general in terms of timing
unsigned char read_channel(char channel_config, volatile char* unit)
{
	*unit = channel_config;
	_delay_us(50);
	return *unit;
}
