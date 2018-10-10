#include "system_constants.h"
#define BAUDRATE 9600
#define UBRR F_CPU/16/BAUDRATE - 1
#include "uart/uart.h"
#include "can/CAN_driver.h"
#include <stdio.h> //Trengs denne?
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "../node1/usb_multifunction_card/joystick.h"
//#include "utils/uart.h"

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))

int MY_PINS[8] = {PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7};

void init_all(void)
{
	uart_init(UBRR);
	printf("Uart init\n");
	CAN_init();
}

int main()
{
	/* Initialize system */
	init_all();
	_delay_ms(1000);
	printf("\n\n\nNode 2 Initialized\n\n\n");

	while(1){
		CAN_message received_msg;
		CAN_receive(&received_msg);

		joystick_status status = {
			received_msg.data[0],
			received_msg.data[1],
			received_msg.data[2],
			received_msg.data[3],
		};

		printf("Joystick_status: x: %3d, y: %3d, pressed: %d, dir: %1d \n",
			status.x, status.y, status.pressed, status.dir);
	};
    
}

