#define F_CPU 4915200
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "oled.h"
#include "uart/uart.h"
#include "fonts.h"

#define N_PAGES 8
#define N_COLUMNS 128

/* 
Timer1 resolution is 16
Generate interrput every 16.6 ms (60Hz)
(4915200/1024)/60 = 80
*/
#define OLED_BUFFER_SIZE 1024
#define CHAR_LENGTH 8
#define FONT_NAME font8

static volatile char* OLED_CMD = (volatile char*) 0x1000; // A9 0
static volatile char* OLED_DATA = (volatile char*) 0x1200; // A9 1
static volatile char * OLED_BUFFER = (volatile char*) 0x1C00;

struct oled_data_marker_struct
{
    char page; /* Current page */
    char column; /* Current column */
    int changed; /* Dont flush SRAM if nothing have changed */
};

static struct oled_data_marker_struct oled_data;

static void write_command(unsigned int command)
{
    cli();
    *OLED_CMD = command;
    sei();
}

static void write_data(unsigned int data)
{
    cli();
    oled_data.changed = 1;
    uint16_t index = oled_data.page*N_COLUMNS + oled_data.column;
    OLED_BUFFER[index] = data;
    if (++oled_data.column == N_COLUMNS){
        oled_data.column = 0;
        if (++oled_data.page == N_PAGES)
            oled_data.page = 0;
    }
    sei();
}

void oled_set_page(unsigned int page)
{
    if (page >= N_PAGES){
        printf("Illegal page access");
        while(1);
    }

    oled_data.page = page;

}

void oled_set_column(unsigned int column)
{
    if (column >= N_COLUMNS){
        printf("Illegal column access");
        while(1);
    }

    oled_data.column = column;
}

void oled_clear_screen(void)
{
    cli();
    for (int i = 0; i < N_PAGES; ++i)
    {
        /* Set start address to 0, end andress to 7F (128) */
        oled_set_column(0);
        oled_set_page(i);
        for (int i = 0; i < N_COLUMNS; ++i)
        {
            write_data(0);
        }
    }
    sei();
}

// Will clear an area of the OLED (including the _end column or page)
void oled_clear_area(
    const int page_start,
    const int page_end,
	const int column_start,
    const int column_end)
{
    cli();
    for (int i = page_start; i < page_end - page_start + 1; i++)
    {
	oled_set_page(i);
	oled_set_column(column_start);

	int n = 0;
	while (n < column_end - column_start + 1)
	{
	    write_data(0);
	    ++n;
	}
    }
    sei();
}

static void print_char(const char c)
{
    for (int i = 0; i < CHAR_LENGTH; i++) {
        write_data(pgm_read_byte(&FONT_NAME[c - 32][i]));
    }
}

void oled_printf(const char* string)
{
    char* stringPtr = string;

    while(*stringPtr != '\0') {
        print_char(*stringPtr);
        ++stringPtr;
    }
}

void oled_display_image(
    const char* imgname,
    unsigned int size,
    unsigned int page,
    unsigned int column)
{
    oled_clear_screen();
    /* Find start address and set up input buffer bypass */
    volatile char * buffer = OLED_BUFFER + page*N_COLUMNS + column;
    uart_write_image_to_SRAM(buffer, size);

    /* Request image */
    printf("@i %s\n", imgname);
}

void oled_init(void) {
    // INIT function from the data sheet
    write_command(0xAE); // display off
    write_command(0xA1); //segment remap
    write_command(0xDA); //common pads hardware: alternative
    write_command(0x12);
    write_command(0xC8); //common output scan direction:com63~com0
    write_command(0xA8); //multiplex ration mode:63
    write_command(0x3F);
    write_command(0xD5); //display divide ratio/osc. freq. mode
    write_command(0x80);
    write_command(0x81); //contrast control
    write_command(0x50);
    write_command(0xD9); //set pre-charge period
    write_command(0x21);
    write_command(0x20); //Set Memory Addressing Mode
    write_command(0x00); // Horizontal addressing mode
    write_command(0xDB); //VCOM deselect level mode
    write_command(0x30);
    write_command(0xAD); //master configuration
    write_command(0x00);
    write_command(0xA4); //out follows RAM content
    write_command(0xA6); //set normal display
    write_command(0xAF); // display on

    oled_clear_screen();

    oled_set_column(0);
    oled_set_page(0);

    oled_data.page = 0;
    oled_data.column = 0;
    oled_data.changed = 0;

    /* Set up timer interrupt */ 
    /* Normal operation */
    TCCR1A = 0;

    /* 1024 prescalar */
    TCCR1B = (1 << CS10 | (1 << CS12));

    /* Load Compare registers */
    OCR1AH = 0;
    OCR1AL = 160;

    /* Enable interrupt */
    TIMSK |= (1 << OCIE1A);

}

ISR(TIMER1_COMPA_vect)
{
    /* Check if flush is needed */
    if (!oled_data.changed)
        return;
    /* Move to (0, 0) of oled */
    write_command(0x21);
    write_command(0);
    write_command(N_COLUMNS - 1);
    write_command(0x22);
    write_command(0);
    write_command(N_PAGES - 1);

    /* Write buffer */
    for (uint16_t i = 0; i < OLED_BUFFER_SIZE; ++i)
    {
        *OLED_DATA = OLED_BUFFER[i];
    }
    //oled_data.changed = 0;
    /* Is it neccecary to clear high register? */
    TCNT1H = 0;
    TCNT1L = 0;
}

