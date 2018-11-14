#include "ir_driver.h"
#include "adc_driver.h"

#define THRESHOLD 70

static uint8_t n_goals = 0;
static uint8_t scoring_enabled = 0;

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
	
	uint8_t scored = adc_read() < THRESHOLD;
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