#include <avr/io.h> //Trengs denne?
#include <stdio.h>
#include "uart.h"


#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))


void uart_init(const unsigned int ubrr)
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr >> 8); // High bits of counter
	UBRR0L = (unsigned char)ubrr; // Low bits of counter

	// Burde disse bytte plass?
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

	//fdevopen(uart_send_char, uart_recieve_char);	
}

int uart_send_char(char c, FILE* dummy)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)));

	/* Put data into buffer, sends the data */
	UDR0 = c;

	return 0;
}


int uart_recieve_char(FILE* dummy)
{
	/* Wait for data to be received */
	while(!(UCSR0A & (1 << RXC0)));

	/* Get and return received data from buffer */
	return UDR0;
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
