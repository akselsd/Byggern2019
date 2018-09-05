#define F_CPU 4915200
#define BAUDRATE 9600
#define UBRR F_CPU/16/BAUDRATE - 1
#include <stdio.h> //Trengs denne?
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))


int main()
{
	CLEAR_BIT(DDRD, PIN0); //0 for input
	SET_BIT(DDRD, PIN1); //1 for output

	uart_init(UBRR);

	char c = 'c';
	char * a = {"hei"};
	
	while(1){
		//printf(a);
		//_delay_ms(100);
		//printf(c);
		char d = fgetc(stdin);
		_delay_ms(100);
		uart_send_char(d, NULL);
		_delay_ms(100);
		//printf(c);
		/*
		char c = uart_recieve_char(NULL);
		_delay_ms(100);
		uart_send_char(c, NULL);
		_delay_ms(100);
		*/
	}
}