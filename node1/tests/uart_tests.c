#include "tests.h"
#include <stdio.h>
#include <assert.h>

int test_uart_ouput_streams(void)
{
	printf("Testing stdout\n");
	fprintf(stderr, "Testing stderr\n");
	printf("Testing number formatting %.2f\n", 3.14159);
	return 0;
}