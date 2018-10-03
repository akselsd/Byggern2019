#define F_CPU 4915200
#define BAUDRATE 9600
#define UBRR F_CPU/16/BAUDRATE - 1
#include <stdio.h> //Trengs denne?
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "tests/tests.h"
#include "utils/uart.h"
#include "can/CAN_driver.h"
#include "tests/sram_test.h"
#include "usb_multifunction_card/buttons.h"
#include "usb_multifunction_card/joystick.h"
#include "usb_multifunction_card/usb_multifunction_card_io.h"
#include "usb_multifunction_card/oled.h"
#include "usb_multifunction_card/menu.h"

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))

int MY_PINS[8] = {PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7};

void init_all(void)
{
	/* Initialize external memory adressing */
	SET_BIT(MCUCR, SRE);

	uart_init(UBRR);

	// Currently does nothing
	//joystick_calibrate_joystick();

	usb_multifunction_buttons_init();

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

	//while(1)
	{
		
		CAN_message msg;
		msg.id = 1;
		msg.data[0] = 69;
		msg.data[1] = 202;
		msg.data[2] = 5;		
		msg.data[3] = '¿';		
		msg.data[4] = 5;		
		msg.data[5] = 5;		
		msg.data[6] = 5;		
		msg.data[7] = 2;		
		msg.length = 8;
		CAN_send(&msg);
		printf("CAN message sent.\n");
		_delay_ms(1000);

		CAN_message received_msg;
		CAN_receive(&received_msg);
		printf("CAN message received.\n");
		printf("%d\n", received_msg.data[0]);
		printf("%d\n", received_msg.data[1]);
		printf("%d\n", received_msg.data[2]);
		printf("%d\n", received_msg.data[3]);
		printf("%d\n", received_msg.data[4]);
		printf("%d\n", received_msg.data[5]);
		printf("%d\n", received_msg.data[6]);
		printf("%d\n", received_msg.data[7]);
	}
	while(1){
		menu_select(&current_menu_choice);
		_delay_ms(100);
	};
    

    /*
  	volatile char * SRAM = 0x1800;
	volatile char * ADC = 0x1400;
	volatile char * OLED_DATA = 0x1200;
	volatile char * OLED_COMMAND = 0x1000;
	while(1){
		SRAM[1] = 69;
	}*/
}
//DDRx input/output. 1 = output, 0 = input
//PORTx Write for output, pull up for input
//PINx Read for input

// Sram 1000 0000 0000 to 1111 1111 1111
// ADC 0100 0000 0000 to 0111 1111 1111
// OLED_DATA 0010 0000 0000 to 0011 1111 1111
// OLED_COMMAND 0000 0000 0000 to 0001 1111 1111
//A11 1: SRAM -> Set P19 high (CS2)
// P18: ADC
// P17: OLED_DATA
// P16: OLED_COMMAND
//A10 and not A11: ADC
//A9 and not A11 and not A10: OLED_DATA
//not A11 and not A10 and not A9: OLED_COMMAND
