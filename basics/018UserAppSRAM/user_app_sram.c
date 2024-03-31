#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "usart_driver.h"
#include "common_utils.h"
#include "general_purpose_timer.h"

extern struct USART_Handle_t Test_USART;

uint32_t loop_counter = 100000;

int main(void)
{
	//Configuring the timer
	configure_delay_timer();

	//Configuring the UART
	configure_uart();

	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT);

	printf("Initial value of loop counter is %ld\r\n",loop_counter);
	printf("Starting the upcounting...\r\n");

	/* Loop forever */
	while(1)
	{
		GPIOTogglePin(GPIOA, GPIO_PIN_5);
		printf("Current value of loop counter is %ld\r\n",loop_counter++);
		delay_us(100000);
	}

	return 0;
}
