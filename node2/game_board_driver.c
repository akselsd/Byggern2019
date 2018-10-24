#include "game_board_driver.h"
#include "pwm_driver.h"
#include "ir_driver.c"

static void update_pwm(CAN_message * msg)
{
	float x = msg->data[1];
	/* Map [0, 255] linearly to [-100, 100] */
	x = x/1.275;
	x = x - 100;
	pwm_set_duty_cycle(x);
}

void game_board_receive(CAN_message * msg)
{
	update_pwm(msg);
}

void game_board_shoot(void)
{
	// TODO: Shoot here
	ir_enable();

}