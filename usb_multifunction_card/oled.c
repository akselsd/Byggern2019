#define F_CPU 4915200
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "oled.h"
#include "fonts.h"

#define N_PAGES 8
#define N_COLUMNS 128

#define CHAR_LENGTH 8
#define FONT_NAME font8

volatile char* OLED_CMD = (volatile char*) 0x1000; // A9 0
volatile char* OLED_DATA = (volatile char*) 0x1200; // A9 1

static void write_command(unsigned int command)
{
    *OLED_CMD = command;
}

static void write_data(unsigned int data)
{
    *OLED_DATA = data;
}

void oled_set_page(unsigned int page)
{
    if (page > N_PAGES){
        printf("Illegal page access");
        while(1);
    }

    write_command(0x22);
    write_command(page);
    write_command(N_PAGES - 1);
}

void oled_set_column(unsigned int column)
{
    if (column > N_COLUMNS){
        printf("Illegal column access");
        while(1);
    }

    write_command(0x21);
    write_command(column);
    write_command(N_COLUMNS - 1);
}

void oled_clear_screen(void)
{
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
}

// Will clear an area of the OLED (including the _end column or page)
void oled_clear_area(const int page_start, const int page_end,
	const int column_start, const int column_end)
{
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
}
