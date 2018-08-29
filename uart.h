#ifndef INCLUDE_UART_H
#define INCLUDE_UART_H

//Make interrupt?

void uart_init(void);
void uart_send_char(const unsigned char c);
//void uart_send_string(const unsigned char * c);
unsigned char uart_recieve_char(void);

#endif