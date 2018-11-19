#include "system_constants.h"
#include "bit_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "pwm_driver.h"

void pwm_init(void)
{
    cli();
    
    // Enable PWM output on Timer 4A, PH3
    SET_BIT(DDRH, PH3);

    // Set timer mode to mode 14: Fast PWM mode
    CLEAR_BIT(TCCR4A, WGM10);
    SET_BIT(TCCR4A, WGM11);
    SET_BIT(TCCR4B, WGM12);
    SET_BIT(TCCR4B, WGM13);

    // Set prescaler of clock to be 8 for channel B
    CLEAR_BIT(TCCR4B, CS10);
    SET_BIT(TCCR4B, CS11);
    CLEAR_BIT(TCCR4B, CS12);

    // Set PWM output mode to be non-inverted for channel B
    SET_BIT(TCCR4A, COM4B1);
    CLEAR_BIT(TCCR4A, COM4B0);
    
    pwm_set_freq(440);
    pwm_set_duty_cycle(50);

    sei();
}

void pwm_set_freq(uint16_t freq)
{
    // Set TOP reg to contain correct value for desired freq
    ICR4 = F_CPU / (PRESCALER * freq) - 1;
}

void pwm_set_duty_cycle(float percentage) {
	if ((percentage < -100) || (percentage > 100))
	{	
		printf("Error: Duty cycle percentage out of range: %d\n", (int)percentage);
		return;
	}

	/* Map [-100, 100] linearly to [0.9 / 20, 2.1 / 20] */
	float duty_cycle = PWM_CENTER_PERCENTAGE + PWM_GAIN_PERCENTAGE * percentage / 100;
	//printf("PWM duty_cycle: %d\n", (int)(duty_cycle*100));
	duty_cycle *= top;
	cli();
	OCR1B = (uint16_t) duty_cycle;
	sei();
}
