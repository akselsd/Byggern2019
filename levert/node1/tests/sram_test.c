#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "uart/uart.h"
#include "sram_test.h"

void SRAM_test(void)
{
    volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\n");

    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();
    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    //while(1){
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];

        if (retreived_value != some_value) {
            //printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n",
            //    i, retreived_value, some_value);
            write_errors++;
        }
        //else{
        	//printf("Success\n");
        //}
    }
    //}


    // Retrieval phase: Check that no values were changed
    // during or after the write phase
    srand(seed);

    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];

        if (retreived_value != some_value) {
            //printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", 
            //    i, retreived_value, some_value);
            retrieval_errors++;
        }
    }

    printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n",
            write_errors, retrieval_errors);
}

void my_sram_test()
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting write 255 SRAM test...\n");
    for (uint16_t i = 0; i < ext_ram_size; ++i)
    {
    	ext_ram[i] = 13;
    	if (ext_ram[i] != 13)
    		printf("Error in reg: %4d\n", i);
    }
    for (uint16_t i = 0; i < ext_ram_size; ++i)
    {
    	uint8_t retreived_value = ext_ram[i];
    	printf("Value: %4d, read %4d\n",retreived_value, i);
    }
}

void print_byte_as_bits(char val) {
  for (int i = 7; 0 <= i; i--) {
    printf("%c", (val & (1 << i)) ? '1' : '0');
  }
}
