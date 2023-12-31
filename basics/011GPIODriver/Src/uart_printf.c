/*
 * task_scheduler.c
 *
 *  Created on: 11-Sep-2023
 *      Author: naveen
 */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "usart_driver.h"
#include "gpio_driver.h"

#define UART_TX_PIN			GPIO_PIN_2
#define UART_RX_PIN			GPIO_PIN_3
#define SLEEP_COUNT			500000

struct USART_Handle_t Test_USART;

void configure_uart(void);
void UART_SendChar(uint8_t ch);
void delay(void);

int main(void)
{
	uint32_t count = 0;
	configure_uart();

	printf("Hello Brave New World!!!\r\n");

	while(1)
	{
		//printf("Hello World!!!\r\n");
		//delay();
	}

	return 0;
}

void configure_uart(void)
{
	//GPIO Pin Configuration
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA,UART_TX_PIN,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOA,UART_RX_PIN,GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,UART_TX_PIN,GPIO_ALTFN_7);
	GPIOSetAltFn(GPIOA,UART_RX_PIN,GPIO_ALTFN_7);
	GPIOSetOutputType(GPIOA,UART_TX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputType(GPIOA,UART_RX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputSpeed(GPIOA,UART_TX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetOutputSpeed(GPIOA,UART_RX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(GPIOA,UART_TX_PIN,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(GPIOA,UART_RX_PIN,GPIO_PULL_UP);

	//USART Configuration
	Test_USART.pUSART = (struct USART_RegDef_t *) USART2;
	Test_USART.USART_Config.USART_Mode = USART_MODE_TX_RX;
	Test_USART.USART_Config.USART_DataLength = USART_DATA_LEN_8_BITS;
	Test_USART.USART_Config.USART_StopBits = USART_STOP_BITS_1;
	Test_USART.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	Test_USART.USART_Config.USART_BaudRate = USART_SB_RATE_9600;
	Test_USART.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CNTRL_NONE;

	EnablePeriClk(USART2);
	USART_Init(&Test_USART);
	USART_PeripheralEnable(&Test_USART);
}

void UART_SendChar(uint8_t ch)
{
	USART_SendData(&Test_USART, &ch, 1);
}

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}
