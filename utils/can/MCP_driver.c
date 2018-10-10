#define F_CPU 4915200
#include "SPI_driver.h"
#include "MCP_driver.h"
#include "MCP2515.h"
#include <util/delay.h>
#include <stdio.h>

void MCP_init(void)
{
	SPI_master_init();
	printf("SPI Init Complete\n");
	MCP_reset(); // Does not set mode to config
	printf("MCP Reset\n");
	/* Wait for reset */
	_delay_ms(100);

	MCP_write(MCP_CANCTRL, MODE_LOOPBACK);
	_delay_ms(100);
	printf("Reading status\n");
	printf("Status: %d\n", MCP_read_status());

	MCP_write(MCP_CANCTRL, MODE_NORMAL);
	_delay_ms(100);
	printf("Status: %d\n", MCP_read_status());

}

void MCP_reset(void)
{
	SPI_start_transmit();
	//_delay_ms(10); TODO is this necessary?
	SPI_write(MCP_RESET);
	SPI_end_transmit();
}

uint8_t MCP_read(uint8_t address)
{
	SPI_start_transmit();

	uint8_t data;
	SPI_write(MCP_READ);
	SPI_write(address);

	data = SPI_read();

	SPI_end_transmit();
	return data;
}

void MCP_read_n(uint8_t address, uint8_t * data, uint8_t n_bytes)
{
	SPI_start_transmit();

	SPI_write(MCP_READ);
	SPI_write(address);
	for (int i = 0; i < n_bytes; ++i)
	{
		data[i] = SPI_read();
	}

	SPI_end_transmit();
}

void MCP_write(uint8_t address, uint8_t data)
{
	SPI_start_transmit();

	//_delay_ms(10); TODO is this necessary?
	SPI_write(MCP_WRITE);
	SPI_write(address);
	SPI_write(data);
	SPI_end_transmit();
}

void MCP_write_n(uint8_t address, uint8_t * data, uint8_t n_bytes)
{
	SPI_start_transmit();
	SPI_write(MCP_WRITE);
	SPI_write(address);
	for (int i = 0; i < n_bytes; ++i)
	{
		SPI_write(data[i]);
	}

	SPI_end_transmit();
}

void MCP_request_to_send(void)
{
	SPI_start_transmit();

	SPI_write(MCP_RTS_TX0);

	SPI_end_transmit();
}

uint8_t MCP_read_status(void)
{
	/* Returns MCP_CANSTAT reg. */
	return MCP_read(MCP_READ_STATUS);
}

void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
	SPI_start_transmit();

	SPI_write(MCP_BITMOD);
	SPI_write(address);
	SPI_write(mask);
	SPI_write(data);

	SPI_end_transmit();
}



