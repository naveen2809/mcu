#include <stdint.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "flash_driver.h"

#define SLEEP_COUNT	50000
#define LEN			64

void delay(void);

uint8_t TxBuf[LEN];
uint8_t RxBuf[LEN];

int main(void)
{

	uint32_t i;

	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT);

	for(i=0;i<LEN;i++)
	{
		TxBuf[i] = 'C';
	}
	
	flash_sector_erase(2);
	flash_write(TxBuf,(uint8_t*)0x08008000,LEN);
	flash_read(RxBuf,(uint8_t*)0x08008000,LEN);

	/* Loop forever */
	while(1)
	{
		GPIOTogglePin(GPIOA, GPIO_PIN_5);
		delay();
	}

	return 0;
}

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}