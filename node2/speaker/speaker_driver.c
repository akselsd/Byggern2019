#include "speaker_driver.h"
#include "pwm_driver.h"

void speaker_init(void)
{
    pwm_init();
}

void speaker_play_song(void)
{
    pwm_set_freq();
}
