#define F_CPU 4915200
#define BAUDRATE 9600
#define UBRR F_CPU/16/BAUDRATE - 1
#include <stdio.h> //Trengs denne?
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
//#include "utils/uart.h"

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))

int MY_PINS[8] = {PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7};

void init_all(void)
{
	//uart_init(UBRR);
}

int main()
{
	/* Initialize system */
	init_all();
	_delay_ms(1000);
	printf("\n\n\nInitialized\n");

	while(1){
		printf("UART WORKING.\n");
	};
    
}

