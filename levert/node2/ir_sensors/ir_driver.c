#include "system_constants.h"
#include <util/delay.h>
#include <stdio.h>

#include "ir_driver.h"
#include "adc_driver.h"

#define N_SAMPLES 5
#define TRESHOLD_RATE 0.10

static uint16_t treshold;

void ir_init(void)
{
	adc_init();

	/* Calculate treshold */
	uint16_t avg_samples = 0;

	for (uint8_t n = 0; n < N_SAMPLES; ++n)
	{
		avg_samples += adc_read();
		_delay_ms(20);
	}

	avg_samples /= N_SAMPLES;

	treshold = avg_samples * TRESHOLD_RATE;

	printf("Treshold calculated: %d\n", treshold);
}

uint8_t ir_check_block(void)
{
	return adc_read() < treshold;
}