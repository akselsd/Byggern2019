#define F_CPU 4915200
#define BAUD 9600
#include <util/setbaud.h>
#include <avr/io.h> //Trengs denne?
#include "uart.h"

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))


void uart_init(void)
{
	UCSRC &= ~BV(UMSEL); // Ascync mode
	UCSRC &= ~((1 << UPM1) | (1 << UMP0));
	
	SET_BIT(UCSRC, UCSZ1);
	SET_BIT(UCSRC, UCSZ0);
	CLEAR_BIT(UCSRB, UCSZ2);
		
	UBRRH = UBBRH_VALUE; // High bits of counter
	UBBRL = UBBRL_VALUE; // Low bits of counter

	UCSRB |= _BV(RXEN) | _BV(TXEN);

	//UCSRB |= (1 << RXCIE); // Enable interrupt on RXC
	//UCSRB |= (1 << TXCIE); // Enable interrupt on TXC
	//UCSRB |= (1 << UDRIE); // Enable interrput on UDRE
}

void uart_send_char(const unsigned char c)
{
	while (!(UCSRA & (1 << UDRE)));

	UDR = c;
}

unsigned char uart_recieve_char(void)
{
	while(!(USCRA & (1 << RXC)))

	return UDR;
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