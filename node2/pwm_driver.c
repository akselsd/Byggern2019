#include "pwm_driver.h"
#include "bit_macros.h"
#include "system_constants.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

/*
#define WGM10 0
#define WGM11 1
#define WGM12 3
#define WGM13 4

#define COM1A0 6
#define COM1A1 7

#define CS10 0
#define CS11 1
#define CS12 2
*/

#define F_PWM 50
#define PWM_CENTER_PERCENTAGE 1.5 * F_PWM
#define PWM_GAIN_PERCENTAGE 0.6 * F_PWM 
#define PRESCALER 8

static uint16_t top; 

void pwm_init(void) {
	/* Enable PWM output */
	SET_BIT(DDRB, PB5);

	/* Set timer mode to mode 14: Fast PWM mode */
	CLEAR_BIT(TCCR1A, WGM10);
	SET_BIT(TCCR1A, WGM11);
	SET_BIT(TCCR1B, WGM12);
	SET_BIT(TCCR1B, WGM13);

	/* Clock select: Set prescaler of clock to be 8 */
	CLEAR_BIT(TCCR1B, CS10);
	SET_BIT(TCCR1B, CS11);
	CLEAR_BIT(TCCR1B, CS12);

	/* Calculate top counter value based on desired PWM frequency */
	top = F_CPU / (PRESCALER * F_PWM) - 1;
	printf("PWM top: %d\n", top);

	pwm_set_duty_cycle(0);

	/* Set PWM output mode to be non-inverted */
	SET_BIT(TCCR1A, COM1A1);
	CLEAR_BIT(TCCR1A, COM1A0);
}

void pwm_set_duty_cycle(float percentage) {
	if ((percentage < -100) || (percentage > 100))
	{	
		fprintf(stderr, "Error: Duty cycle percentage out of range.");
		return;
	}

	/* Map [-100, 100] linearly to [0.9 / 20, 2.1 / 20] */
	float duty_cycle_percentage = PWM_CENTER_PERCENTAGE + PWM_GAIN_PERCENTAGE * percentage / 100;
	duty_cycle_percentage *= top;
	printf("PWM duty_cycle_percentage: %.3f\n", duty_cycle_percentage);

	OCR1A = (uint16_t) duty_cycle_percentage;
}