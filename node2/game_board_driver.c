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
#include "../node1/usb_multifunction_card/joystick.h"
#include "../node1/fsm.h"

#define SOLENOID PC6

static uint8_t scoring_enabled;
static uint8_t communication_enabled;

static uint8_t current_diff = DIFF_EASY; // Defaults to EASY

static void update_pwm(uint8_t x_int)
{
	float x = x_int;
	/* Map [0, 255] linearly to [-100, 100] */
	x = x/1.275;
	x = x - 100;
	//printf("Attempting to set PWM signal to %d\n", (uint8_t)x);
	pwm_set_duty_cycle(x);
}

static void controll_game_board(CAN_message * msg)
{
    switch(current_diff)
    {
	case DIFF_EASY:
	    game_board_shoot(msg->data[5], msg->data[3]); // Joystick UP, L button pressed
	    // Controll controller input with joystick x
	    controller_set_input(msg->data[0]); 
	    break;
	case DIFF_MEDIUM:
	    update_pwm(msg->data[0]); // Joystick x
	    controller_set_reference(msg->data[7]); // Slider right
	    game_board_shoot(msg->data[5], msg->data[3]); // Joystick UP, L button pressed
	    break;
	case DIFF_HARD:
	    update_pwm(msg->data[0]); // Joystick x
	    controller_set_reference(255 - msg->data[7]); // Invert reference
	    game_board_shoot(msg->data[5], msg->data[3]); // Joystick UP, L button pressed
	    break;
    }
}

void game_board_reset(uint8_t player_diff)
{
	communication_enabled = 0;
	printf("Reset game board\n");

	current_diff = player_diff;

	controller_clear();

	// Calibrate dc_motor
	motor_box_set_direction(MOTOR_LEFT);
	motor_box_set_speed(100);
	_delay_ms(1000);
	motor_box_set_speed(0);
	_delay_ms(400);
	motor_box_reset_encoder();

	scoring_enabled = 0;

	controller_init(current_diff == DIFF_EASY);
	communication_enabled = 1;
}

void game_board_init(void)
{
	cli();

	printf("Initialize game board\n");
	game_board_reset(DIFF_EASY);

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
		switch (msg->id)
		{
			case ID_RESET_GB:
				if ((msg->data[0]) && (msg->length == 2))
					game_board_reset(msg->data[1]); // msg->data[1] - diff
				break;
			case ID_IO:
				controll_game_board(msg);
				break;
			default:
				printf("Unknown CAN message ID: %u\n", msg->id);
				break;
		}
	}
}

void game_board_shoot(uint8_t r_pressed, uint8_t joystick_dir)
{
	if (r_pressed || (joystick_dir == UP))
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
	CAN_message msg;
	msg.id = ID_GOAL;
	CAN_send(&msg);
}
