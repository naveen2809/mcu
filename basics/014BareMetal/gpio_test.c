#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"

#define SLEEP_COUNT	50000

uint32_t toggle_count = 0;
uint32_t status;

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{

	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT);
	//GPIOSetMode(GPIOD, GPIO_PIN_13, GPIO_MODE_OUTPUT);
	//GPIOSetMode(GPIOD, GPIO_PIN_14, GPIO_MODE_OUTPUT);
	//GPIOSetMode(GPIOD, GPIO_PIN_15, GPIO_MODE_OUTPUT);

	/* Loop forever */
	while(1)
	{
		GPIOTogglePin(GPIOA, GPIO_PIN_5);
		//GPIOTogglePin(GPIOD, GPIO_PIN_13);
		//GPIOTogglePin(GPIOD, GPIO_PIN_14);
		//GPIOTogglePin(GPIOD, GPIO_PIN_15);
		delay();
		toggle_count++;
		status = toggle_count % 2;

		printf("Hello World!\r\n");
	}

	return 0;
}
