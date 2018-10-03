#ifndef MCP_DRIVERS_H_INCLUDED
#define MCP_DRIVERS_H_INCLUDED
#include <stdint.h>

void MCP_init(void);
void MCP_reset(void);
uint8_t MCP_read(uint8_t address);
void MCP_read_n(uint8_t address, uint8_t * data, uint8_t n_bytes);
void MCP_write(uint8_t address, uint8_t data);
void MCP_write_n(uint8_t address, uint8_t * data, uint8_t n_bytes);
void MCP_request_to_send(void);
uint8_t MCP_read_status(void);
void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data);

#endif
