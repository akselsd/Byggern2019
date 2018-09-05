#ifndef INCLUDE_UART_H
#define INCLUDE_UART_H
#include <stdio.h>

void uart_init(const unsigned int ubrr);
int uart_flush_send_buffer(void);

/* Functions not used outside uart.c Make static and hide from interface? */
int uart_send_char(char c, FILE* neverused);
int uart_recieve_char(FILE* neverused);
#endif
