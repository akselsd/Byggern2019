#include <stdio.h>
#include <stdint.h>
#include "../can/MCP_drivers.h"
#include "tests.h"

void test_MCP_read(uint8_t address)
{
	uint8_t data = MCP_read(address);
	printf("Data: %u", data);
}
