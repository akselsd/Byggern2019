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
	volatile char* ADC = 0x1000;
	while(1){
		ADC[1] = 69;
	}
}
//DDRx input/output. 1 = output, 0 = input
//PORTx toggle high/low for output, read from input
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

