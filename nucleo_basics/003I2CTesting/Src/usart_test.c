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


#define UART_TX_PIN				GPIO_PIN_9
#define UART_RX_PIN				GPIO_PIN_10
#define GPIO_PIN_NUM_INPUT		GPIO_PIN_13
#define GPIO_PIN_NUM_OUTPUT		GPIO_PIN_5
#define SLEEP_COUNT				500000
#define ENABLE					1
#define USART2_IRQ_NUM			38

struct USART_Handle_t Test_USART;
char TxBuf[32] = "UART Tx from Discovery Board!\r\n";
char RxBuf[32];
#define MessageLen				31

void USART2_IRQHandler(void)
{
	USART_IRQHandling(&Test_USART);
}

void USART_RXNEInterruptCallback(uint8_t data)
{
	printf("%c\r\n",data);
}

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
	Test_USART.pUSART = (struct USART_RegDef_t *) USART1;
	Test_USART.USART_Config.USART_Mode = USART_MODE_TX_RX;
	Test_USART.USART_Config.USART_DataLength = USART_DATA_LEN_8_BITS;
	Test_USART.USART_Config.USART_StopBits = USART_STOP_BITS_1;
	Test_USART.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	Test_USART.USART_Config.USART_BaudRate = USART_SB_RATE_9600;
	Test_USART.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CNTRL_NONE;


	EnablePeriClk(USART1);
	USART_Init(&Test_USART);
	USART_PeripheralEnable(&Test_USART);
	//USART_EnableTXEInterrupt(&Test_USART);
	//USART_EnableRXNEInterrupt(&Test_USART);
	//NVIC_EnableIRQ(USART2_IRQ_NUM);


	for(;;)
	{

		/* UART Tx Using Polling*/

		if(GPIOReadPin(GPIOC,GPIO_PIN_NUM_INPUT) == GPIO_LOW)
		{
			GPIOTogglePin(GPIOA,GPIO_PIN_NUM_OUTPUT);
			USART_SendData(&Test_USART, (uint8_t *) TxBuf, (uint32_t) strlen(TxBuf));
			delay();
		}

		/* UART Rx Using Polling*/


		/*
		USART_SendData(&Test_USART, (uint8_t *) TxBuf, (uint32_t) strlen(TxBuf));
		USART_ReceiveData(&Test_USART, (uint8_t *) RxBuf, RxBufLen);

		for(i=0;i<RxBufLen;i++)
			printf("%c",RxBuf[i]);

		printf("\r\n");

		current_value = ((*pGPIOD_ODR >> GPIO_PIN_NUM_OUTPUT) & 0x1);
		current_value ^= 0x1;
		*pGPIOD_ODR &= ~(1 << GPIO_PIN_NUM_OUTPUT);
		*pGPIOD_ODR |= (current_value << GPIO_PIN_NUM_OUTPUT);
		delay();

		*/

		/*
		while(!received);

		printf("%c\r\n",RxBuf[0]);
		received = 0;
		delay();
		*/
	}

	USART_PeripheralDisable(&Test_USART);
	DisablePeriClk(USART1);

	return 0;
}
