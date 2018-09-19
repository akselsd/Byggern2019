#define F_CPU 4915200

#include <util/delay.h>
#include "usb_multifunction_card_io.h"

unsigned char read_channel(char channel_config, volatile char* unit)
{
	*unit = channel_config;
	_delay_us(50);
	return *unit;
}
