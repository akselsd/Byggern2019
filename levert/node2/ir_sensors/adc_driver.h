#ifndef ADC_DRIVER_H_INCLUDED
#define ADC_DRIVER_H_INCLUDED
#include <stdint.h>

void adc_init(void);
uint16_t adc_read(void);

#endif