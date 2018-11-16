#include "system_constants.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "game_board_driver.h"
#include "servo_motor/pwm_driver.h"
#include "ir_sensors/ir_driver.h"
#include "dc_motor/motor_box_driver.h"
#include "bit_macros.h"

#define ENCODER_RANGE 8380
#define SCALING_FACTOR 32.8627
#define INTERRUPT_PERIOD 0.0326568
#define SOLENOID PC6

typedef struct motor_input_struct
{
	uint8_t speed;
	motor_direction dir;
} motor_input;

typedef struct controller_values_struct
{
	uint8_t reference;
	float error_sum;
	float old_error;
	motor_input output;
	
	float kp;
	float ki;
	float kd;
} controller_values;

static volatile controller_values controller;

static void update_pwm(CAN_message * msg)
{
	float x = msg->data[1];
	/* Map [0, 255] linearly to [-100, 100] */
	x = x/1.275;
	x = x - 100;
	//printf("Attempting to set PWM signal to %d\n", (uint8_t)x);
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

static void update_motor_box(CAN_message * msg)
{
	//printf("Slider values: Left %u, Right %u -- ", msg->data[0], msg->data[1]);
	//printf("Encoder value: %d\n", motor_box_read());

	/* Enable or disable motor with left slider */
	if (msg->data[0] < 200){
		motor_box_set_speed(0);
		return;
	}

	cli();
	controller.reference = msg->data[1];
	//printf("Setting controller refference to %d\n", msg->data[1]);
	sei();
}

static void controller_clear(void)
{

	//clear controller values
	controller.kp = 0;
	controller.ki = 0;
	controller.kd = 0;

	/* Set controller values */
	controller.reference = 0;
	controller.error_sum = 0;
	controller.old_error = 0;

	/* Disable overflow interrupts */
	CLEAR_BIT(TIMSK3, TOIE3);

	printf("Controller off\n");
}

static void controller_init(void)
{
	/* Set controller values */
	controller.reference = 0;
	controller.error_sum = 0;
	controller.old_error = 0;

	/* Set controller parameters */
	controller.kp = 1.4;
	controller.ki = 0.8;
	controller.kd = 0.1;

	/* Enable overflow interrupts */
	SET_BIT(TIMSK3, TOIE3);
	
	printf("Controller on\n");
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

	/* Set up timer 3 in CTC mode with compare pin OCR3A*/
	CLEAR_BIT(TCCR3B, WGM33);
	CLEAR_BIT(TCCR3B, WGM32);
	CLEAR_BIT(TCCR3A, WGM31);
	CLEAR_BIT(TCCR3A, WGM30);

	/* Set prescaler to 8 */
	CLEAR_BIT(TCCR3B, CS32);
	SET_BIT(TCCR3B, CS31);
	CLEAR_BIT(TCCR3B, CS30);

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
			update_motor_box(msg);
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

/* Calculate controller values */
ISR(TIMER3_OVF_vect) {
	/* Map motor position [0,-8700] to [0,255] */
	float current_position = -motor_box_read() / SCALING_FACTOR;
	float error = controller.reference - current_position;

	printf("-");

	controller.error_sum += error*INTERRUPT_PERIOD;

	float input = controller.kp * error + controller.ki * controller.error_sum + controller.kd * (error - controller.old_error) / INTERRUPT_PERIOD;
	input = saturate_input(input);

	controller.old_error = error;
	
	if (input > 0)
	{
		controller.output.speed = (uint8_t)input;
		controller.output.dir = MOTOR_RIGHT;
	}
	else
	{
		controller.output.speed = (uint8_t)(-1*input);
		controller.output.dir = MOTOR_LEFT;
	}

	motor_box_set_speed(controller.output.speed);
	motor_box_set_direction(controller.output.dir);	
}