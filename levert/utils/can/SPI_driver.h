#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H
#include <stdint.h>

void SPI_master_init(void);
void SPI_start_transmit(void);
void SPI_end_transmit(void);
void SPI_write(uint8_t data);
uint8_t SPI_read(void);



#endif
