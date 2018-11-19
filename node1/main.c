#include "system_constants.h"
#define BAUDRATE 9600
#define UBRR F_CPU/16/BAUDRATE - 1
#include <stdio.h> //Trengs denne?
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "bit_macros.h"
#include "can/CAN_driver.h"
#include "tests/tests.h"
#include "tests/sram_test.h"
#include "uart/uart.h"
#include "usb_multifunction_card/buttons.h"
#include "usb_multifunction_card/joystick.h"
#include "usb_multifunction_card/usb_multifunction_card_io.h"
#include "usb_multifunction_card/oled.h"
#include "usb_multifunction_card/slider.h"
#include "usb_multifunction_card/menu.h"
#include "fsm.h"

void init_all(void)
{
	//TODO check further SRAM?? What is this
	SET_BIT(MCUCR, SRE);

	uart_init(UBRR);
	usb_multifunction_buttons_init();
	usb_multifunction_joystick_init();
	menu_init();
	oled_init();
	CAN_init();
	load_leaderboard();
}



int main()
{
	/* Initialize system */
	init_all();
	_delay_ms(1000);
	printf("\n\n\nInitialized\n");
	oled_clear_screen();
	fsm_main_loop();
}


