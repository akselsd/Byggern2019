#ifndef SPEAKER_PWM_DRIVER_H
#define SPEAKER_PWM_DRIVER_H

#include <stdint.h>

void pwm_init(void);
void pwm_set_freq(uint16_t freq);
void pwm_set_duty_cycle(float percentage);

#endif
