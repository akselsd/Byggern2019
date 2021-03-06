#ifndef INCLUDE_UART_H
#define INCLUDE_UART_H
#include <stdio.h> 

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

/**
 * Bypass the input buffer and write directily to SRAM
 * Used for loading images from computer
*/
void uart_write_image_to_SRAM(volatile char * buffer, unsigned int img_size);

/**
 * Bypass the input buffer and write directily to SRAM
 * Used for loading leaderboard from computer
*/
volatile char ** uart_write_leaderboard_RAM(void);

/* Functions not used outside uart.c Make static and hide from interface? */
int uart_send_char(char c, FILE* neverused);
int uart_recieve_char(FILE* neverused);


#endif
