#include "system_constants.h"
#define BAUDRATE 9600
#define UBRR F_CPU/16/BAUDRATE - 1

#include <stdio.h> //Trengs denne?
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "uart/uart.h"
#include "can/CAN_driver.h"
#include "game_board_driver.h"

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))

int MY_PINS[8] = {PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7};

void init_all(void)
{
	uart_init(UBRR);
	printf("Uart init\n");
	CAN_init();
	pwm_init();
}

int main()
{
	/* Initialize system */
	init_all();
	_delay_ms(1000);
	printf("\n\n\nNode 2 Initialized\n\n\n");

	while(1){
		CAN_message * msg = CAN_receive();
		printf("x: %d\n", msg->data[1]);
		game_board_receive(msg);
		CAN_message_destructor(msg);
		_delay_ms(50);

		//printf("Joystick_status: x: %3d, y: %3d, pressed: %d, dir: %1d \n",
			//status.x, status.y, status.pressed, status.dir);
	};
    
}

