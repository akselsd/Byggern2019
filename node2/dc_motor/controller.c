#include <avr/io.h>
#include <avr/interrupt.h>
#include "controller.h"
#include "bit_macros.h"

#define ENCODER_RANGE 8380
#define SCALING_FACTOR 32.862745098
#define INTERRUPT_PERIOD 0.0326568

#define KP 1.4
#define KI 1.2
#define KD 0.1

static volatile controller_values controller;

static float saturate_input(float input)
{
	if (input < -255)
		return -255.0;

	if (input > 255)
		return 255.0;

	return input;
}

void controller_init(uint8_t ext_input_ctrl)
{
	/* Set controller values */
	controller.reference = 0;
	controller.error_sum = 0;
	controller.old_error = 0;
	controller.input = 0;
	controller.ext_input_ctrl = ext_input_ctrl;

	/* Set controller parameters */
	controller.kp = KP;
	controller.ki = KI;
	controller.kd = KD;

	/* Enable overflow interrupts */
	SET_BIT(TIMSK3, TOIE3);

	/* Set up timer 3 in CTC mode with compare pin OCR3A*/
	CLEAR_BIT(TCCR3B, WGM33);
	CLEAR_BIT(TCCR3B, WGM32);
	CLEAR_BIT(TCCR3A, WGM31);
	CLEAR_BIT(TCCR3A, WGM30);

	/* Set prescaler to 8 */
	CLEAR_BIT(TCCR3B, CS32);
	SET_BIT(TCCR3B, CS31);
	CLEAR_BIT(TCCR3B, CS30);
}

void controller_clear(void)
{
	/*clear controller parameters*/
	controller.kp = 0;
	controller.ki = 0;
	controller.kd = 0;

	/* Set controller values */
	controller.reference = 0;
	controller.error_sum = 0;
	controller.old_error = 0;
	controller.input = 0;
	controller.ext_input_ctrl = 0;

	/* Disable overflow interrupts */
	CLEAR_BIT(TIMSK3, TOIE3);
}

void controller_set_reference(uint8_t ref)
{
	/* Enable or disable motor with left slider */
	/*if (msg->data[0] < 20)
	{
		motor_box_set_speed(0);
		return;
	}*/

	cli();
	controller.reference = ref;
	sei();
}

void controller_set_input(float input)
{
	// Map [0, 255] to [-127.0, 127.0]	
	controller.input = saturate_input(input * 2 - 255) / 2;
}

ISR(TIMER3_OVF_vect) {
	if (!controller.ext_input_ctrl)
	{
		/* Map motor position [0,-8700] to [0,255] */
		controller.current_position = motor_box_read() / SCALING_FACTOR;
		float error = controller.reference - controller.current_position;

		controller.error_sum += error*INTERRUPT_PERIOD;

		controller.input = controller.kp * error + controller.ki * controller.error_sum
			+ controller.kd * (error - controller.old_error) / INTERRUPT_PERIOD;
		controller.input = saturate_input(controller.input);

		controller.old_error = error;
	}
	
	// Convert controller.input to correct output values for motor box
	if (controller.input > 0)
	{
		controller.output.speed = (uint8_t)controller.input;
		controller.output.dir = MOTOR_RIGHT;
	}
	else
	{
		controller.output.speed = (uint8_t)(-1*controller.input);
		controller.output.dir = MOTOR_LEFT;
	}

	motor_box_set_speed(controller.output.speed);
	motor_box_set_direction(controller.output.dir);	
}
