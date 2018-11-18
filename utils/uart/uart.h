#ifndef INCLUDE_UART_H
#define INCLUDE_UART_H
#include <stdio.h> // FILE

#define BUFFER_SIZE 64
#define PAGE_LENGTH 128

typedef struct ringbuffer_struct
{
	volatile char buffer[BUFFER_SIZE];
	volatile int next_out;
	volatile int next_in;
	volatile int size;
} ringbuffer;

typedef struct image_buffer_struct
{
	volatile char * buffer;
	volatile unsigned int n_bytes;
	volatile unsigned int n_pages;
	volatile unsigned int size;
} image_buffer;

typedef struct leaderboard_buffer_struct
{
	volatile char * buffer;
	volatile uint8_t n_bytes;
	volatile uint8_t n_lines;
	volatile uint8_t line_length;
} leaderboard_buffer;


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
void uart_write_leaderboard_to_SRAM(volatile char * buffer, uint8_t line_length, uint8_t n_lines);

/* Functions not used outside uart.c Make static and hide from interface? */
int uart_send_char(char c, FILE* neverused);
int uart_recieve_char(FILE* neverused);
#endif
