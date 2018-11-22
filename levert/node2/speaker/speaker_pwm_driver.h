#ifndef SPEAKER_PWM_DRIVER_H
#define SPEAKER_PWM_DRIVER_H

#include <stdint.h>

void speaker_pwm_init(void);
void speaker_pwm_set_freq(uint16_t freq);
void speaker_pwm_enable(void);
void speaker_pwm_disable(void);
void speaker_pwm_set_duty_cycle(float percentage);

#endif
