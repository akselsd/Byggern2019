#ifndef INCLUDE_UART_H
#define INCLUDE_UART_H
#include <stdio.h>
//Make interrupt?

void uart_init(const unsigned int ubrr);
int uart_send_char(char c, FILE* neverused); // Gjøre static?
int uart_recieve_char(FILE* neverused); // Gjøre static?
int uart_flush_send_buffer(void);


#endif