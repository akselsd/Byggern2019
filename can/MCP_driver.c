#define F_CPU 4915200
#include "SPI_driver.h"
#include "MCP_driver.h"
#include "MCP2515.h"
#include <avr/delay.h>
#include <stdio.h>


void MCP_write(uint8_t address, uint8_t data)
{
	SPI_start_transmit();
	_delay_ms(10);
	SPI_transmit(MCP_WRITE);
	SPI_transmit(address);
	SPI_transmit(data);
	SPI_end_transmit();
}


uint8_t MCP_read(uint8_t address)
{
	uint8_t data;
	SPI_start_transmit();
	SPI_transmit(MCP_READ);
	data = SPI_transmit(address);
	SPI_end_transmit();
	return data;
}

void MCP_init(void)
{
	printf("%d\n", MCP_read(MCP_CANSTAT));
	SPI_start_transmit();
	_delay_ms(10);
	SPI_transmit(MCP_RESET);
	printf("%d\n", MCP_read(MCP_CANSTAT));
	SPI_end_transmit();
	MCP_write(MCP_CANCTRL, MODE_NORMAL);
	printf("%d\n", MCP_read(MCP_CANSTAT));
	_delay_ms(1000);
	printf("%d\n", MCP_read(MCP_CANSTAT));
}




