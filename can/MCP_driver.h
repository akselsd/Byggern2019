#ifndef MCP_DRIVERS_H_INCLUDED
#define MCP_DRIVERS_H_INCLUDED
#include <stdint.h>

uint8_t MCP_read(uint8_t address);
void MCP_write(uint8_t address, uint8_t data);

#endif