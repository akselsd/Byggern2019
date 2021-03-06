#include "system_constants.h"

#define BAUDRATE 9600
#define UBRR F_CPU/16/BAUDRATE - 1

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ir_sensors/adc_driver.h"
#include "bit_macros.h"
#include "can/CAN_driver.h"
#include "dc_motor/motor_box_driver.h"
#include "game_board_driver.h"
#include "servo_motor/pwm_driver.h"
#include "uart/uart.h"
#include "ir_sensors/ir_driver.h"
#include "speaker/speaker_driver.h"

void init_all(void)
{
	// BE CAREFUL WHEN CHANGING ORDER!!
	uart_init(UBRR);
	printf("Uart init\n");
	CAN_init();
	pwm_init();
	motor_box_init();
	_delay_ms(200);
	game_board_init();
	_delay_ms(200);
	ir_init();
}

int main()
{
	/* Initialize system */
	cli();
	init_all();
	_delay_ms(1000);
	printf("\n\n\nNode 2 Initialized\n\n\n");
	sei();

	while(1)
	{
		if(game_board_check_goal())
		{	
			game_board_transmit_goal();
			speaker_life_lost();
		}
	}
}


/* Receive message interrupt */
ISR(INT4_vect)
{
 	CAN_message received_msg;
 	CAN_receive(&received_msg);
	game_board_handle_msg(&received_msg);
}
