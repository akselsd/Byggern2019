#include <avr/io.h> //Trengs denne?
#include <stdio.h>
#include <stdlib.h> // Abort
#include <avr/interrupt.h>
#include "uart.h"
#define BUFFER_SIZE 256


#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))

struct ringbuffer
{
	char buffer[BUFFER_SIZE];
	int next_out;
	int next_in;
	int size;
};

struct ringbuffer send_buffer = {{0}, 0, 0, 0};
struct ringbuffer recieve_buffer = {{0}, 0, 0, 0};


void uart_init(const unsigned int cpu_frq, const unsigned int baudrate)
{
	int ubrr = cpu_frq/16/baudrate - 1;
	UBRR0H = (unsigned char)(ubrr >> 8); // High bits of counter
	UBRR0L = (unsigned char)ubrr; // Low bits of counter

	// Burde disse bytte plass?
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00); //Endre til UCSZ01 og UCSZ00?
	UCSR0B |= (1 << RXCIE0); //Recieve interrupt
	sei(); // Enable interrupts
	fdevopen(uart_send_char, uart_recieve_char);	
}

// Sjekke errorflag?
int uart_send_char(char c, FILE* dummy)
{
	cli();
	if (send_buffer.size >= BUFFER_SIZE)
		abort(); // Handle overflow here. Return 1 to fdevopen?

	if (send_buffer.size == 0)
		UCSR0B |= (1 << UDRIE0); //Enable Transmit register empty interrupt

	send_buffer.buffer[send_buffer.next_in++] = c;
	++send_buffer.size;
	

	if (send_buffer.next_in >= BUFFER_SIZE)
		send_buffer.next_in = 0;

	sei();
	return 0; // Success
}

// Sjekke errorflag?
int uart_recieve_char(FILE* dummy)
{
	cli();
	// Block if there is no current input
	while (recieve_buffer.size == 0);

	char c = recieve_buffer.buffer[recieve_buffer.next_out++];
	if (recieve_buffer.next_out == BUFFER_SIZE)
		recieve_buffer.next_out = 0;

	--recieve_buffer.size;
	sei();
	return c;
}

ISR(USART0_RXC_vect)
{
	if (recieve_buffer.size == BUFFER_SIZE)
		abort(); // Handle overflow here;

	recieve_buffer.buffer[recieve_buffer.next_in++] = UDR0;
	if (recieve_buffer.next_in == BUFFER_SIZE)
		recieve_buffer.next_in = 0;

	++recieve_buffer.size;
}

ISR(USART0_UDRE_vect)
{
	UDR0 = send_buffer.buffer[send_buffer.next_out++];
	if (send_buffer.next_out == BUFFER_SIZE)
		send_buffer.next_out = 0;
	
	--send_buffer.size;
	if (send_buffer.size == 0)
		UCSR0B &= ~(1 << UDRIE0); //Disable Transmit register empty interrupt

}
//TXEN Transmit Enable
//UMSEL bit in UCSRC velger Async/Sync (0 for Async)
//UBRR
//UDR = Register to send data (Transmit buffer)
//UDRE USART Data Register Empty (Ready to recieve new data, set when empty)
//Write to zero when writing to USCRA
//TXC Transmit Complete
//Set when completed with frame in Transmit Shift Register
//Cleared by writing one to its bit location
//RXEN Recieve Enable
//RXC Ready to recieve, set when ready