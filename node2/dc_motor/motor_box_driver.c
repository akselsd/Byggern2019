#include "system_constants.h"
#include <avr/io.h>
#include <util/delay.h>
#include "motor_box_driver.h"
#include "dac_driver.h"
#include "bit_macros.h"
#include <stdio.h>

#define DIR PH1
#define SEL PH3
#define EN PH4
#define _OE PH5 // Active low
#define _RST PH6 // Active low

#define DO0 PK0
#define ENCODER_MAX 8380
#define ENCODER_MIN 0
#define DEADZONE 200

void motor_box_init(void)
{
	/* Enable output on pins connected to MJ1 header */
	SET_BIT(DDRH, DIR);
	SET_BIT(DDRH, SEL);
	SET_BIT(DDRH, EN);
	SET_BIT(DDRH, _OE);
	SET_BIT(DDRH, _RST);

	/* Enable input on all pins connected to MJ2 header */
	DDRK = 0;

	dac_init();

	/* Disable servo */
	SET_BIT(PORTH, EN);

	/* Reset encoder*/
	CLEAR_BIT(PORTH, _RST);
	_delay_us(100);
	SET_BIT(PORTH, _RST);

	/* Disable encoder output */
	SET_BIT(PORTH, _OE);

	/* Direction default */
	CLEAR_BIT(PORTH, DIR);

	motor_box_set_speed(0);
}

void motor_box_reset_encoder(void)
{
	CLEAR_BIT(PORTH, _RST);
	_delay_us(20);
	SET_BIT(PORTH, _RST);	
}

static void saturate_output(int16_t * encoder_output)
{
	if (*encoder_output > ENCODER_MAX)
	{
		*encoder_output = ENCODER_MAX;
	}
	else if (*encoder_output < ENCODER_MIN)
	{
		*encoder_output = ENCODER_MIN;
	}
}

int16_t motor_box_read(void)
{
	int16_t encoder_output;

	/* Enable output */
	CLEAR_BIT(PORTH, _OE);

	/* Read MSB */
	CLEAR_BIT(PORTH, SEL);
	_delay_us(200);
	encoder_output = PINK << 8;

	/* Read LSB */
	SET_BIT(PORTH, SEL);
	_delay_us(200);
	encoder_output += PINK;
	
	/* Disable output */
	SET_BIT(PORTH, _OE);

	encoder_output *= -1;

	saturate_output(&encoder_output);

	return encoder_output;
}

void motor_box_set_direction(motor_direction dir)
{
	dir == MOTOR_LEFT ? CLEAR_BIT(PORTH, DIR) : SET_BIT(PORTH, DIR);
}

void motor_box_set_speed(uint8_t speed)
{
	dac_set_output(speed);
}
