#ifndef INCLUDE_UART_H
#define INCLUDE_UART_H

//Make interrupt?

void uart_init(const unsigned int cpu_frq, const unsigned int baudrate);
int uart_send_char(char c, FILE* neverused); // Gjøre static?
int uart_recieve_char(FILE* neverused); // Gjøre static?


#endif