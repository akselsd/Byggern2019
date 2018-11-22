#ifndef DAC_DRIVER_H_INCLUDED
#define DAC_DRIVER_H_INCLUDED

#include <stdint.h>

void dac_init(void);
void dac_disable(void);
void dac_set_output(uint8_t value);

#endif