#ifndef INCLUDE_UART_H
#define INCLUDE_UART_H
#include <stdio.h> // FILE

/**
 * Initialize uart communication and provide streams for stdout, stderr and stdin.
 * @param ubrr Baud Rate Register Value defined as CPU_FREQUENCY/(16*BAUDRATE) - 1.
*/
void uart_init(const unsigned int ubrr);

/**
 * Flush the send buffer
 * Will send all chars currently in sending buffer. Blocks until the buffer is empty.
 * @return 0 for success.  
*/
int uart_flush_send_buffer(void);

void uart_write_input_to_buffer(char * buffer, unsigned int n_bytes)

/* Functions not used outside uart.c Make static and hide from interface? */
int uart_send_char(char c, FILE* neverused);
int uart_recieve_char(FILE* neverused);
#endif
