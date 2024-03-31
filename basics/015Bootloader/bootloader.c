#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "usart_driver.h"
#include "common_utils.h"
#include "bootloader_utils.h"
#include "external_interrupt_config.h"

extern char CmdBuffer[];
extern uint32_t CmdLen;
extern struct USART_Handle_t Test_USART;

#define LED_LOOP_COUNT	200

#define NUCLEO_BTN_INT_PORT			GPIOC
#define NUCLEO_BTN_INT_PIN			GPIO_PIN_13
#define NUCLEO_BTN_INT_TRIGGER		EXTI_RISING_TRIGGER_ENABLE
#define NUCLEO_BTN_INT_IRQ_NUM		40

uint8_t run_bootloader = 0;

static void bootloader_main_loop(void);

int main(void)
{

	uint32_t i;

	//Configuring the timer
	configure_delay_timer();

	//Configuring the UART
	configure_uart();

	//Configure the SPI Peripheral
	configure_spi();

	//Configure the FLASH Module
	at45dbxx_configure_flash();

	//Configuring LED GPIO
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT);

	//Enable button interrupt
	configure_external_gpio_interrupt(NUCLEO_BTN_INT_PORT,NUCLEO_BTN_INT_PIN,NUCLEO_BTN_INT_TRIGGER,NUCLEO_BTN_INT_IRQ_NUM);

	for(i=0;i<LED_LOOP_COUNT;i++)
	{
		
		printf("%ld\r\n",i);

		GPIOTogglePin(GPIOA, GPIO_PIN_5);
		
		if(run_bootloader == 1)
		{
			//Disable button interrupt
			NVIC_DisableIRQ(NUCLEO_BTN_INT_IRQ_NUM);
			
			//Call bootloader main loop
			bootloader_main_loop();
		}
			
		delay_us(100000);
	}

	//Disable button interrupt
	NVIC_DisableIRQ(NUCLEO_BTN_INT_IRQ_NUM);

	//Disable Timer, UART and LED GPIO

	//Call application
	handle_command_start_app();

	return 0;
}

static void bootloader_main_loop(void)
{
	
	GPIOWritePin(GPIOA, GPIO_PIN_5,GPIO_LOW);
	
	setbuf(stdout, NULL);

	printf("\r\n");

	while(1)
	{
		printf("$ ");
		USART_ReceiveCmd(&Test_USART,(uint8_t *)CmdBuffer,&CmdLen);
		if(CmdLen > 1)
		{
			handle_command();
		}
	}

	return;
}

void EXTI15_10_Handler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	//Handling the Interrupt
	run_bootloader = 1;

	//Clearing the Button Interrupt
	*pEXTI_PR |= (1 << NUCLEO_BTN_INT_PIN);		// Clearing the EXTI_PR Register

	return;
}