#include "system_constants.h"
#include "bit_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "speaker_pwm_driver.h"

#define PRESCALER 8

static uint16_t top;

void speaker_pwm_init(void)
{
    cli();
    
    // Enable PWM output on Timer 4B, PH3
    SET_BIT(DDRH, PH4);

    // Set timer mode to mode 14: Fast PWM mode
    CLEAR_BIT(TCCR4A, WGM10);
    SET_BIT(TCCR4A, WGM11);
    SET_BIT(TCCR4B, WGM12);
    SET_BIT(TCCR4B, WGM13);

    // Set prescaler of clock to be 8 for channel B
    CLEAR_BIT(TCCR4B, CS10);
    SET_BIT(TCCR4B, CS11);
    CLEAR_BIT(TCCR4B, CS12);

    speaker_pwm_set_freq(440);
    speaker_pwm_set_duty_cycle(100);

    // Set PWM output mode to be non-inverted for channel B
    SET_BIT(TCCR4A, COM4B1);
    CLEAR_BIT(TCCR4A, COM4B0);

    sei();
}

void speaker_pwm_set_freq(uint16_t freq)
{
    // Calc TOP value
    top = F_CPU / (PRESCALER * freq) - 1;

    // Set TOP reg to contain correct value for desired freq
    ICR4 = top;
}

void speaker_pwm_set_duty_cycle(float percentage) {
	if ((percentage < -100) || (percentage > 100))
	{	
		printf("Error: Duty cycle percentage out of range: %d\n", (int)percentage);
		return;
	}

	float duty_cycle = percentage / 100;
	duty_cycle *= top;
	cli();
	OCR4B = (uint16_t) duty_cycle;
	sei();
}
