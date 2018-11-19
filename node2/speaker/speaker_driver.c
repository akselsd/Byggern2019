#include "speaker_driver.h"
#include "speaker_pwm_driver.h"

void speaker_init(void)
{
    speaker_pwm_init();
}

void speaker_play_song(void)
{
    speaker_pwm_set_freq(440);
}
