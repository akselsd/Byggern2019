#define F_CPU 4915200
#define BAUD 9600
#include <util/setbaud.h>
#include <avr/io.h> //Trengs denne?
#include "uart.h"

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))


void uart_init(void)
{
	UCSR0C |= (1 << URSEL0); //Write to UCSR0C
	UCSR0C &= ~(1 << UMSEL0); // Ascync mode
	UCSR0C &= ~((1 << UPM01) | (1 << UPM00));
	UCSR0C &= ~(1 << USBS0);
	UCSR0C &= ~((1 << UCSZ01) | (1 << UCSZ00));
	
		
	UBRR0H = UBRRH_VALUE; // High bits of counter
	UBRR0L = UBRRL_VALUE; // Low bits of counter

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

	//UCSRB |= (1 << RXCIE); // Enable interrupt on RXC
	//UCSRB |= (1 << TXCIE); // Enable interrupt on TXC
	//UCSRB |= (1 << UDRIE); // Enable interrput on UDRE
}

void uart_send_char(const char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

unsigned char uart_recieve_char(void)
{
	while(!(UCSR0A & (1 << RXC0)))

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