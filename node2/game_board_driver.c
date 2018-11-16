#include "system_constants.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "game_board_driver.h"
#include "servo_motor/pwm_driver.h"
#include "ir_sensors/ir_driver.h"
#include "bit_macros.h"

#define SOLENOID PC6

static uint8_t n_goals = 0;

static void update_pwm(CAN_message * msg)
{
	float x = msg->data[1];
	/* Map [0, 255] linearly to [-100, 100] */
	x = x/1.275;
	x = x - 100;
	//printf("Attempting to set PWM signal to %d\n", (uint8_t)x);
	pwm_set_duty_cycle(x);
}

void game_board_reset(void)
{
	printf("Reset game board\n");

	controller_clear();

	/* Calibrate servo */
	motor_box_set_direction(MOTOR_LEFT);
	motor_box_set_speed(100);
	_delay_ms(1000);
	motor_box_reset_encoder();
	motor_box_set_speed(0);
	controller_init();
}

void game_board_init(void)
{
	cli();

	printf("Initialize game board\n");
	game_board_reset();

	/* Enable solenoid */
	SET_BIT(DDRC, SOLENOID);
	SET_BIT(PORTC, SOLENOID);

	sei();
}

void game_board_handle_msg(CAN_message * msg)
{
	//printf("ID: %d --", msg->id);
	switch (msg->id){
		case ID_JOYSTICK:
			update_pwm(msg);
			break;
		case ID_SLIDERS:
			controller_set_reference(msg);
			break;
		case ID_BUTTONS:
			game_board_shoot(msg);
			break;
		case ID_RESET:
			game_board_reset();
			break;
		default:
			printf("Unknown CAN message ID: %u\n", msg->id);
			break;
	}
}

void game_board_shoot(CAN_message * msg)
{
	/* Right button */
	if (msg->data[1]){
		ir_enable();
		CLEAR_BIT(PORTC, SOLENOID);
		_delay_ms(5);
	}
	else
		SET_BIT(PORTC, SOLENOID);
}

uint8_t game_board_check_goal(void)
{
	return ir_check_block();
}

uint8_t game_board_get_goal_count(void)
{
	return n_goals;
}
