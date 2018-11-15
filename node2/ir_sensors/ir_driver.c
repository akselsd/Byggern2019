#include "system_constants.h"
#include <util/delay.h>
#include <stdio.h>

#include "ir_driver.h"
#include "adc_driver.h"

#define N_SAMPLES 5
#define TRESHOLD_RATE 0.78

static uint8_t treshold;
static uint8_t n_goals = 0;
static uint8_t scoring_enabled = 0;

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
	
	ir_enable();
}

void ir_enable(void)
{
	scoring_enabled = 1;
}

void ir_disable(void)
{
	scoring_enabled = 0;
}

uint8_t ir_check_goal(void)
{
	if (!scoring_enabled)
		return 0;
	
	uint8_t scored = adc_read() < treshold;
	//printf("%d\n", adc_read());
	if (scored)
		ir_disable();
		++n_goals;
	return scored;
}

uint8_t ir_get_goal_count(void)
{
	return n_goals;
}