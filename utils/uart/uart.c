#include "system_constants.h"
#include <avr/io.h> //Trengs denne?
#include <stdio.h>
#include <stdlib.h> // Abort
#include <stdio.h>
#include <avr/interrupt.h>

#include "bit_macros.h"
#include "uart.h"

#define BUFFER_SIZE 64
#define PAGE_LENGTH 128

#define LEADERBOARD_SIZE 32

struct ringbuffer
{
	volatile char buffer[BUFFER_SIZE];
	volatile int next_out;
	volatile int next_in;
	volatile int size;
};

struct image_buffer
{
	volatile char * buffer;
	volatile unsigned int n_bytes;
	volatile unsigned int n_pages;
	volatile unsigned int size;
};

/*typedef struct leaderboard_buffer_struct
{
	volatile char * buffer;
	volatile uint8_t n_bytes;
	volatile uint8_t n_lines;
	volatile uint8_t lines_left;
	volatile uint8_t line_length;
} leaderboard_buffer;
*/

struct ringbuffer send_buffer = {{0}, 0, 0, 0};
struct ringbuffer recieve_buffer = {{0}, 0, 0, 0};
struct image_buffer img_buffer = {NULL, 0, 0, 0};
//static leaderboard_buffer lb_buffer = {NULL, 0, 0, 0, 0};

volatile char leaderboard_data[33];
volatile uint8_t leaderboard_load = 0;
volatile uint8_t leaderboard_current_char = 0;

void uart_init(const unsigned int ubrr)
{
	cli();
	CLEAR_BIT(DDRD, PIN0); //0 for input
	SET_BIT(DDRD, PIN1); //1 for output

	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr >> 8); // High bits of counter
	UBRR0L = (unsigned char)ubrr; // Low bits of counter

	// Burde disse bytte plass?
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<UART_DATA_BIT)|(1<<USBS0)|(3<<UCSZ00); //Endre til UCSZ01 og UCSZ00?
	UCSR0B |= (1 << RXCIE0); //Recieve interrupt
	sei();
	fdevopen(uart_send_char, uart_recieve_char);	
}

// Sjekke errorflag?
int uart_send_char(char c, FILE* dummy)
{
	cli();
	/* If buffer is full, force flush */
	if (send_buffer.size >= BUFFER_SIZE)
		uart_flush_send_buffer();

	/*  If buffer is empty, enable Transmit Register Empty Interrupt */
	if (send_buffer.size == 0)
		UCSR0B |= (1 << UDRIE0);

	/* Put data into buffer */
	send_buffer.buffer[send_buffer.next_in++] = c;
	++send_buffer.size;

	/* Wrap write pointer if necessary*/
	if (send_buffer.next_in >= BUFFER_SIZE)
		send_buffer.next_in = 0;

	/* Add missing carriage return for each newline */
	if (c == '\n')
		uart_send_char('\r', NULL);
	sei();
	return 0; // Success
}

// Sjekke errorflag?
int uart_recieve_char(FILE* dummy)
{
	/* Block if there is no current input */
	while (recieve_buffer.size == 0);
	cli();

	/* Retrieve next char from buffer */
	char c = recieve_buffer.buffer[recieve_buffer.next_out++];

	/* Wrap if necessary */
	if (recieve_buffer.next_out == BUFFER_SIZE)
		recieve_buffer.next_out = 0;

	--recieve_buffer.size;
	sei();

	/* Return received data from buffer */
	return c;
}

/*uint8_t uart_leaderboard_get_n_lines(void)
{
	return lb_buffer.n_lines;
}*/

// UART receive interrupt
ISR(RX_VECTOR)
{
	// Skip intermediate buffer
	if (img_buffer.buffer)
	{
		// Write received byte to buffer
		*(img_buffer.buffer++) = UDR0;

		if (--img_buffer.n_bytes == 0)
		{
			// Image completed
			if (img_buffer.n_pages == 0)
			{

				img_buffer.buffer = NULL;
				//printf("Disable im buffer\n");
			}


			// New page
			else
			{
				--img_buffer.n_pages;
				img_buffer.n_bytes = img_buffer.size;
				img_buffer.buffer+= PAGE_LENGTH - img_buffer.size;
			}
		}
		return;
	}
	
	else if (leaderboard_load)
	{
		leaderboard_data[leaderboard_current_char] = UDR0;


		//if (leaderboard_data[leaderboard_current_char] == '\n')
		//	leaderboard_data[leaderboard_current_char] = '\0';

		//printf("%c\n", leaderboard_data[leaderboard_current_line][leaderboard_current_char]);

		if (++leaderboard_current_char >= 32)
		{
			leaderboard_current_char = 0;
			leaderboard_load = 0;
			printf("LB = 0\n");
			leaderboard_data[32] = '\0';
		}

		return;
	}

	/*if (lb_buffer.buffer)
	{	
		// If lines_left is not defined
		if (lb_buffer.lines_left == 0)
		{
			// Read first byte as n_lines
			lb_buffer.n_lines = UDR0;
			lb_buffer.lines_left = lb_buffer.n_lines;
			return;
		}

		// Load next character into buffer
		*(lb_buffer.buffer++) = UDR0;

		// If end of line is reached
		if (--lb_buffer.n_bytes == 0)
		{
			// Replace '\n' with '\0'
			*(lb_buffer.buffer - 1) = '\0';

			--lb_buffer.lines_left;
			lb_buffer.n_bytes = lb_buffer.line_length;

			// If no more lines to load
			if (lb_buffer.lines_left == 0)
			{
				lb_buffer.buffer = NULL;
				//printf("Disable LB buffer\n");
			}
		}
		return;
	}*/

	if (recieve_buffer.size == BUFFER_SIZE){
		//printf("Recieve buffer overflow\n");
		return;
	}
	/* Write recieved char to buffer */
	recieve_buffer.buffer[recieve_buffer.next_in++] = UDR0;

	/* Wrap if necessary */
	if (recieve_buffer.next_in == BUFFER_SIZE)
		recieve_buffer.next_in = 0;

	++recieve_buffer.size;
}

ISR(USART0_UDRE_vect)
{
	/* Put next char in HW register */
	UDR0 = send_buffer.buffer[send_buffer.next_out++];

	/* Wrap if necessary */
	if (send_buffer.next_out == BUFFER_SIZE)
		send_buffer.next_out = 0;
	
	--send_buffer.size;
	/* If buffer is now empty, disable Transmit Register Empty Interrupt */
	if (send_buffer.size == 0)
		UCSR0B &= ~(1 << UDRIE0); 

}

int uart_flush_send_buffer(void)
{
	cli();
	while (send_buffer.size != 0){
		/* Wait for data to be transmitted */
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = send_buffer.buffer[send_buffer.next_out++];
		--send_buffer.size;

		if (send_buffer.next_out == BUFFER_SIZE)
			send_buffer.next_out = 0;
	}
	/* Buffer empty, disable interrupt */
	UCSR0B &= ~(1 << UDRIE0);
	sei();
	return 0;
}

void uart_write_image_to_SRAM(volatile char * buffer, unsigned int img_size)
{
	cli();
	img_buffer.buffer = buffer;
	img_buffer.n_bytes = img_size;
	img_buffer.size = img_size;
	img_buffer.n_pages = img_size/8;
	sei();
}


volatile char ** uart_write_leaderboard_RAM(void)
{
	leaderboard_load = 1;

	return leaderboard_data;
	/*
	cli();
	UDR0 = 0;
	lb_buffer.buffer = buffer;
	lb_buffer.n_bytes = n_bytes;
	lb_buffer.line_length = n_bytes;
	sei();*/
}