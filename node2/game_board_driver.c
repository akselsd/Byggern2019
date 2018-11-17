#include "system_constants.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "game_board_driver.h"
#include "servo_motor/pwm_driver.h"
#include "ir_sensors/ir_driver.h"
#include "bit_macros.h"
#include "dc_motor/controller.h"
#include "can/CAN_driver.h"

#define SOLENOID PC6

static uint8_t scoring_enabled;
static uint8_t communication_enabled;

static void update_pwm(CAN_message * msg)
{
	float x = msg->data[1];
	printf("%u\n", (uint8_t)msg->data[1]);
	/* Map [0, 255] linearly to [-100, 100] */
	x = x/1.275;
	x = x - 100;
	//printf("Attempting to set PWM signal to %d\n", (uint8_t)x);
	pwm_set_duty_cycle(x);
}

void game_board_reset(void)
{
	communication_enabled = 0;
	printf("Reset game board\n");

	controller_clear();

	/* Calibrate dc_motor */
	motor_box_set_direction(MOTOR_LEFT);
	motor_box_set_speed(100);
	_delay_ms(1000);
	motor_box_set_speed(0);
	_delay_ms(400);
	motor_box_reset_encoder();

	//scoring enabled=0 /why was set to 1?
	scoring_enabled = 0;

	controller_init();
	communication_enabled = 1;
}

void game_board_init(void)
{
	cli();

	printf("Initialize game board\n");
	game_board_reset();

	/* Enable solenoid */
	SET_BIT(DDRC, SOLENOID);
	SET_BIT(PORTC, SOLENOID);

	communication_enabled = 1;


	sei();
}

void game_board_handle_msg(CAN_message * msg)
{
	if (communication_enabled)
	{
		//printf("%d\n", msg->id);
		switch (msg->id)
		{
			case ID_JOYSTICK:
				update_pwm(msg);
				break;
			case ID_SLIDERS:
				controller_set_reference(msg);
				break;
			case ID_BUTTONS:
				game_board_shoot(msg);
				break;
			case ID_RESET_GB:
				game_board_reset();
				break;
			case ID_REQ_GOAL:
				printf("Transmit goal\n");
				game_board_transmit_goal();
				break;
			default:
				printf("Unknown CAN message ID: %u\n", msg->id);
				break;
		}
	}
}

void game_board_shoot(CAN_message * msg)
{
	/* Right button */
	if (msg->data[1])
	{
		if (!scoring_enabled)
			scoring_enabled = 1;

		CLEAR_BIT(PORTC, SOLENOID);
		_delay_ms(5);
	}
	else
		SET_BIT(PORTC, SOLENOID);
}

uint8_t game_board_check_goal(void)
{
	if (!scoring_enabled)
		return 0;

	uint8_t goal = ir_check_block();

	if (goal)
	{
		scoring_enabled = 0;
	}

	return goal;
}

void game_board_transmit_goal()
{
	CAN_message * msg = CAN_message_constructor(ID_GOAL, 1);
	msg->data[0] = game_board_check_goal();
	CAN_send(msg);
	if(msg->data[0])
	{
		printf("GOAL!!!\n\n");
	}
	CAN_message_destructor(msg);
}