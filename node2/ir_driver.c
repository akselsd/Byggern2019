#include "ir_driver.h"
#include "adc_driver.h"

#define THRESHOLD 300

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
		return;
	
	uint8_t scored = adc_read() < THRESHOLD;
	if (scored)
		++n_goals;
	return scored;
}

uint8_t ir_get_goal_count(void)
{
	return n_goals;
}