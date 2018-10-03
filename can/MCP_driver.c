#define F_CPU 4915200
#include "SPI_driver.h"
#include "MCP_driver.h"
#include "MCP2515.h"
#include <avr/delay.h>
#include <stdio.h>


void MCP_write(uint8_t address, uint8_t data)
{
	SPI_start_transmit();

	//_delay_ms(10); TODO is this necessary?
	SPI_transmit(MCP_WRITE);
	SPI_transmit(address);
	SPI_transmit(data);

	SPI_end_transmit();
}


uint8_t MCP_read(uint8_t address)
{
	SPI_start_transmit();

	uint8_t data;
	SPI_transmit(MCP_READ);
	data = SPI_transmit(address);

	SPI_end_transmit();
	return data;
}

void MCP_request_to_sent(uint8_t buffer)
{
	SPI_start_transmit();

	switch(buffer)
	{
		case BUFFER_T0:
			SPI_transmit(MCP_RTS_TX0);
			break;
		case BUFFER_T1:
			SPI_transmit(MCP_RTS_TX1);
			break;
		case BUFFER_T2:
			SPI_transmit(MCP_RTS_TX2);
			break;
		case BUFFER_ALL:
			SPI_transmit(MCP_RTS_ALL);
			break;
				
	}

	SPI_end_transmit();
}

uint8_t MCP_read_status(void)
{
	SPI_start_transmit();

	uint8_t status;
	status = SPI_transmit(MCP_READ_STATUS);

	SPI_end_transmit();

	return status;
}

void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
	SPI_start_transmit();

	SPI_transmit(MCP_BITMOD);
	SPI_transmit(address);
	SPI_transmit(mask);
	SPI_transmit(data);

	SPI_end_transmit();
}

void MCP_init(void)
{
	printf("MCP_CAN_STATUS: %d\n", MCP_read(MCP_CANSTAT));
	printf("Resetting MCP. \n");
	MCP_reset();
	printf("MCP_CAN_STATUS: %d\n", MCP_read(MCP_CANSTAT));

	/* Enter normal mode */
	MCP_write(MCP_CANCTRL, MODE_NORMAL);

	printf("MCP_CAN_STATUS: %d\n", MCP_read(MCP_CANSTAT));
	_delay_ms(1000);
	printf("MCP_CAN_STATUS: %d\n", MCP_read(MCP_CANSTAT));
}

void MCP_reset(void)
{
	SPI_start_transmit();
	//_delay_ms(10); TODO is this necessary?
	SPI_transmit(MCP_RESET);
	SPI_end_transmit();
}

