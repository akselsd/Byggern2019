#include <avr/io.h>
#include "can_driver.h"

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))

void SPI_master_init(void)
{
    /* Enable output on PB4 */ 
    SET_BIT(DDRB, PB4);

    /* Deselect slave */
    SET_BIT(PORTB, PB4);

    /* Set MOSI and SCK output, all others input */
    DDRB = (1 << DDB5) | (1 << DDB7);

    /* Enable SPI, Master, set clock rate fck/16 */
    /* MOSI is automatically set as INPUT when SPI is enabled */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_master_transmit(char data)
{
    /* Select slave */
    CLEAR_BIT(PORTB, PB4);

    /* Start transmission */
    SPDR = data;
    /* Wait for transmission complete */
    while(!(SPSR & (1 << SPIF)))
    ;

    /* Deselect slave */
    SET_BIT(PORTB, PB4);
}
