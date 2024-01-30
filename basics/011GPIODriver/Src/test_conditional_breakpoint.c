/*
 * Conditional Breakpoint
 *
 * Author: naveen
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "common_utils.h"


static void breakpoint(void);

static int volatile count = 0;

int main(void)
{
	//Configure the Timer
	configure_delay_timer();

	//Configure UART
	configure_uart();

	while(1)
	{
		if(count == 100)
		{
			breakpoint();
		}

		printf("Count is %d\r\n", count++);
	}

	return 0;
}

static void breakpoint(void)
{

	return;
}
