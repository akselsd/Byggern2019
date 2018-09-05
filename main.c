#define F_CPU 4915200
#define BAUDRATE 9600
#define UBRR F_CPU/16/BAUDRATE - 1
#include <stdio.h> //Trengs denne?
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "uart.h"
#include "sram_test.h"

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))

int MY_PINS[8] = {PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7};

void test_address(void)
{
	SET_BIT(MCUCR, SRE);
	int * a = 0x1FFF;
	while(1){
		*a = 255;
		_delay_ms(10);
	}	
}

int main()
{
	/* Initialize external memory adressing */
	SET_BIT(MCUCR, SRE);
	uart_init(UBRR);
    //SRAM_test();
	SRAM_test();
	while(1){
		_delay_ms(100);
	}
}
//DDRx input/output. 1 = output, 0 = input
//PORTx toggle high/low for output, read from input
