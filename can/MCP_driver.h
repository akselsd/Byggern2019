#ifndef MCP_DRIVERS_H_INCLUDED
#define MCP_DRIVERS_H_INCLUDED
#include <stdint.h>

enum buffer {BUFFER_T0, BUFFER_T1, BUFFER_T2, BUFFER_ALL};

uint8_t MCP_read(uint8_t address);
void MCP_write(uint8_t address, uint8_t data);
void MCP_request_to_sent(uint8_t buffer);
uint8_t MCP_read_status(void);
void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data);

#endif
