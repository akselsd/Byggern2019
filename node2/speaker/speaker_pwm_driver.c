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
    
    // Enable PWM output on Timer 5A, PL3
    SET_BIT(DDRL, PL3);

    // Set timer mode to mode 14: Fast PWM mode
    CLEAR_BIT(TCCR5A, WGM50);
    SET_BIT(TCCR5A, WGM51);
    SET_BIT(TCCR5B, WGM52);
    SET_BIT(TCCR5B, WGM53);

    // Set prescaler of clock to be 8 for channel A
    CLEAR_BIT(TCCR5B, CS50);
    SET_BIT(TCCR5B, CS51);
    CLEAR_BIT(TCCR5B, CS52);

    speaker_pwm_set_freq(440);
    speaker_pwm_set_duty_cycle(50);

    // Set PWM output mode to be non-inverted for channel A
    SET_BIT(TCCR5A, COM5A1);
    CLEAR_BIT(TCCR5A, COM5A0);

    sei();
}

void speaker_pwm_set_freq(uint16_t freq)
{
    // Calc TOP value
    top = F_CPU / (PRESCALER * freq) - 1;

    // Set TOP reg to contain correct value for desired freq
    ICR5 = top;
}

void speaker_pwm_set_duty_cycle(float percentage) {
	if ((percentage < -100) || (percentage > 100))
	{	
		//printf("Error: Duty cycle percentage out of range: %d\n", (int)percentage);
		return;
	}

	float duty_cycle = percentage / 100;
	duty_cycle *= top;
	cli();
	OCR5A = (uint16_t) duty_cycle;
	sei();
}
