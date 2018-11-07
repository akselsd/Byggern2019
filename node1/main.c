#include "system_constants.h"
#define BAUDRATE 9600
#define UBRR F_CPU/16/BAUDRATE - 1
#include <stdio.h> //Trengs denne?
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

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

void init_all(void)
{
	/* Initialize external memory adressing */
	SET_BIT(MCUCR, SRE);

	uart_init(UBRR);

	// Currently does nothing
	//joystick_calibrate_joystick();

	usb_multifunction_buttons_init();
	usb_multifunction_joystick_init();

	oled_init();
	menu_init();

	CAN_init();
}

int main()
{
	/* Initialize system */
	init_all();
	_delay_ms(1000);
	printf("\n\n\nInitialized\n");

	int current_menu_choice = 0;
	//oled_display_image("mario64", 64, 0, 0);
	while(1){
		joystick_transmit_position();
		_delay_ms(50);
		slider_transmit_position();
		_delay_ms(50);
	};
}