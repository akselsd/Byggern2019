#define F_CPU 4915200
#define BAUDRATE 9600
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))

int main()
{
	CLEAR_BIT(DDRD, PIN0); //0 for input
	SET_BIT(DDRD, PIN1); //1 for output

	uart_init(F_CPU, BAUDRATE);

	while(1){
		uart_send_char('a');
		_delay_ms(100);
	}
}