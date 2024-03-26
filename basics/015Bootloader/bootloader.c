#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "usart_driver.h"
#include "common_utils.h"
#include "bootloader_utils.h"

extern char CmdBuffer[];
extern uint32_t CmdLen;
extern struct USART_Handle_t Test_USART;

#define LED_LOOP_COUNT	20

int main(void)
{

	uint32_t i;

	//Configuring the timer
	configure_delay_timer();

	//Configuring the UART
	configure_uart();

	//Configuring LED GPIO
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT);

	for(i=0;i<LED_LOOP_COUNT;i++)
	{
		GPIOTogglePin(GPIOA, GPIO_PIN_5);
		delay_us(100000);
	}

	setbuf(stdout, NULL);

	printf("\r\n");

	while(1)
	{
		USART_ReceiveCmd(&Test_USART,(uint8_t *)CmdBuffer,&CmdLen);
		if(CmdLen > 1)
		{
			handle_command();
		}
	}

	return 0;
}

