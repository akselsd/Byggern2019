#include "system_constants.h"
#include <avr/io.h>

#include "bit_macros.h"
#include "SPI_driver.h"

void SPI_master_init(void)
{
    /* Set MOSI, SS and SCK output, all others input 
    *  SS needs to be output to avoid others pulling it low
    */
    DDRB = (1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << SS_PIN);
    

    /* Enable output on Select pin */ 
    SET_BIT(DDRB, SELECT_PIN);


    /* Deselect slave */
    SET_BIT(PORTB, SELECT_PIN);
    /* Enable SPI, Master, set clock rate fck/16 */
    /* MOSI is automatically set as INPUT when SPI is enabled */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_start_transmit(void)
{
    /* Select slave - Chip select to low */
    CLEAR_BIT(PORTB, SELECT_PIN);
}

void SPI_end_transmit(void)
{
    /* Deselect slave */
    SET_BIT(PORTB, SELECT_PIN);    
}

void SPI_write(uint8_t data)
{
    SPDR = data;
    while(!(SPSR & (1 << SPIF)));
}

uint8_t SPI_read(void)
{
    SPI_write(0);
    return SPDR;
}
