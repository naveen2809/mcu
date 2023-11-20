/*
 * usart_test.c
 *
 *  Created on: 30-Jul-2023
 *      Author: naveen
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "usart_driver.h"
#include "gpio_driver.h"

#define UART_TX_PIN				GPIO_PIN_10
#define UART_RX_PIN				GPIO_PIN_11
#define GPIO_PIN_NUM_INPUT		GPIO_PIN_13
#define GPIO_PIN_NUM_OUTPUT		GPIO_PIN_5
#define SLEEP_COUNT				500000
#define ENABLE					1

struct USART_Handle_t Test_USART;
char TxBuf[] = "UART message from STM32F446RE Discovery Board!\r\n";
char TxBuf2[] = "Another UART message from the same Board!\r\n";
uint8_t MessageLen[2];

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{
	//GPIO Pin Configuration for Button and LED
	EnablePeriClk(GPIOA);
	EnablePeriClk(GPIOC);
	GPIOSetMode(GPIOA,GPIO_PIN_NUM_OUTPUT,GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOC,GPIO_PIN_NUM_INPUT,GPIO_MODE_INPUT);

	//GPIO Pin Configuration for USART1 Port
	EnablePeriClk(GPIOC);
	GPIOSetMode(GPIOC,UART_TX_PIN,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOC,UART_RX_PIN,GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOC,UART_TX_PIN,GPIO_ALTFN_8);
	GPIOSetAltFn(GPIOC,UART_RX_PIN,GPIO_ALTFN_8);
	GPIOSetOutputType(GPIOC,UART_TX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputType(GPIOC,UART_RX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputSpeed(GPIOC,UART_TX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetOutputSpeed(GPIOC,UART_RX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(GPIOC,UART_TX_PIN,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(GPIOC,UART_RX_PIN,GPIO_PULL_UP);

	//USART Configuration
	Test_USART.pUSART = (struct USART_RegDef_t *) UART4;
	Test_USART.USART_Config.USART_Mode = USART_MODE_TX_RX;
	Test_USART.USART_Config.USART_DataLength = USART_DATA_LEN_8_BITS;
	Test_USART.USART_Config.USART_StopBits = USART_STOP_BITS_1;
	Test_USART.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	Test_USART.USART_Config.USART_BaudRate = USART_SB_RATE_9600;
	Test_USART.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CNTRL_NONE;

	EnablePeriClk(UART4);
	USART_Init(&Test_USART);
	USART_PeripheralEnable(&Test_USART);

	while(1)
	{
		/* UART Tx Using Polling*/

		while(GPIOReadPin(GPIOC,GPIO_PIN_NUM_INPUT) == GPIO_HIGH);
		GPIOTogglePin(GPIOA,GPIO_PIN_NUM_OUTPUT);
		MessageLen[0] = (uint8_t) strlen(TxBuf);
		MessageLen[1] = (uint8_t) strlen(TxBuf);
		USART_SendData(&Test_USART, MessageLen, 2);
		USART_SendData(&Test_USART, (uint8_t *) TxBuf, (uint32_t) strlen(TxBuf));
		delay();

		while(GPIOReadPin(GPIOC,GPIO_PIN_NUM_INPUT) == GPIO_HIGH);
		GPIOTogglePin(GPIOA,GPIO_PIN_NUM_OUTPUT);
		MessageLen[0] = (uint8_t) strlen(TxBuf2);
		MessageLen[1] = (uint8_t) strlen(TxBuf2);
		USART_SendData(&Test_USART, MessageLen, 2);
		USART_SendData(&Test_USART, (uint8_t *) TxBuf2, (uint32_t) strlen(TxBuf2));
		delay();
	}

	USART_PeripheralDisable(&Test_USART);
	DisablePeriClk(UART4);

	return 0;
}
