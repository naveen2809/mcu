#include <stdint.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"

#define SLEEP_COUNT	50000

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{

	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT);

	/* Loop forever */
	while(1)
	{
		GPIOTogglePin(GPIOA, GPIO_PIN_5);
		delay();
	}

	return 0;
}
