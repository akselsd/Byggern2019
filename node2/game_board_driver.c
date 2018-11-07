#include "game_board_driver.h"
#include "pwm_driver.h"
#include "ir_driver.h"
#include "motor_box_driver.h"
#include <stdio.h>

#define ENCODER_RANGE 8380
#define SCALING_FACTOR 32.8627
#define KP 1
#define KI 1
#define KD 1

static void update_pwm(CAN_message * msg)
{
	float x = msg->data[1];
	/* Map [0, 255] linearly to [-100, 100] */
	x = x/1.275;
	x = x - 100;
	pwm_set_duty_cycle(x);
}

static float saturate_input(float input)
{
	if (input < -255)
		return -255.0;

	if (input > 255)
		return 255.0;

	return input;
}

typedef struct motor_input_struct
{
	uint8_t speed;
	motor_direction dir;
} motor_input;

static float old_error = 0;
static float error_sum = 0;
static motor_input current_input;

/* PID controller */
static void set_motor_input(uint8_t target)
{
	float reference = target;
	/* Map motor position [0,-8700] to [0,255] */
	float current_position = -motor_box_read() / SCALING_FACTOR;
	
	float error = reference - current_position;

	//error_sum += error;

	float input = KP * error; // + KI*error_sum + KD*(error - old_error);

	//old_error = error;
	input = saturate_input(input);

	printf("Current position: %d. Target position: %d\n", (int16_t)current_position, (int16_t)reference);
	
	if (input > 0)
	{
		current_input.speed = (uint8_t)input;
		current_input.dir = MOTOR_RIGHT;
	}
	else
	{
		current_input.speed = (uint8_t)(-1*input);
		current_input.dir = MOTOR_LEFT;
	}
}

static void update_motor_box(CAN_message * msg)
{
	//printf("Slider values: Left %u, Right %u -- ", msg->data[0], msg->data[1]);
	//printf("Encoder value: %d\n", motor_box_read());

	/* Enable or disable motor with left slider */
	if (msg->data[0] < 200){
		motor_box_set_speed(0);
		return;
	}

	set_motor_input(msg->data[1]);
	
	/*printf("Dir %s Speed %u\n",
		current_input.dir == MOTOR_LEFT ? "LEFT" : "RIGHT",
		current_input.speed);*/

	motor_box_set_speed(current_input.speed);
	motor_box_set_direction(current_input.dir);	
}

void game_board_handle_msg(CAN_message * msg)
{
	switch (msg->id){
		case ID_JOYSTICK:
			update_pwm(msg);
			break;
		case ID_SLIDERS:
			update_motor_box(msg);
			break;
		case ID_BUTTONS:
			break;
		default:
			printf("Unknown CAN message ID: %u\n", msg->id);
			break;
	}
}

void game_board_shoot(void)
{
	// TODO: Shoot here
	ir_enable();

}