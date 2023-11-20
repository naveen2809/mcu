/*
 * i2c_tx_test.c
 *
 *  Created on: 02-Aug-2023
 *      Author: naveen
 */



#include "stm32f4xx.h"
#include "i2c_driver.h"
#include "usart_driver.h"
#include "gpio_driver.h"
#include <stdint.h>
#include <string.h>

#define UART_TX_PIN						GPIO_PIN_2
#define UART_RX_PIN						GPIO_PIN_3
#define UART_2_TX_PIN					GPIO_PIN_6
#define UART_2_RX_PIN					GPIO_PIN_7
#define GPIO_PIN_NUM_OUTPUT_1			GPIO_PIN_14
#define GPIO_PIN_NUM_OUTPUT_2			GPIO_PIN_15
#define GPIO_PIN_NUM_OUTPUT_3			GPIO_PIN_13
#define SLEEP_COUNT						500000

struct USART_Handle_t Test_USART;
struct USART_Handle_t Test_USART_2;

char message[128];
uint8_t MessageLen[2];

static void NumToString(uint8_t Num, char *NumString);

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{

	char MessageLenString[6] = {48,49,50,'\r','\n'};
	char MessageLenStringPrefix[] = "Message Len: ";

	//GPIO Pin Configuration for LEDs
	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOD,GPIO_PIN_NUM_OUTPUT_1,GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD,GPIO_PIN_NUM_OUTPUT_2,GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD,GPIO_PIN_NUM_OUTPUT_3,GPIO_MODE_OUTPUT);

	//GPIO Pin Configuration for USART2
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

	//GPIO Pin Configuration for USART6
	EnablePeriClk(GPIOC);
	GPIOSetMode(GPIOC,UART_2_TX_PIN,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOC,UART_2_RX_PIN,GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOC,UART_2_TX_PIN,GPIO_ALTFN_8);
	GPIOSetAltFn(GPIOC,UART_2_RX_PIN,GPIO_ALTFN_8);
	GPIOSetOutputType(GPIOC,UART_2_TX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputType(GPIOC,UART_2_RX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputSpeed(GPIOC,UART_2_TX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetOutputSpeed(GPIOC,UART_2_RX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(GPIOC,UART_2_TX_PIN,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(GPIOC,UART_2_RX_PIN,GPIO_PULL_UP);

	//USART2 Port Configuration
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

	//USART2 Port Configuration
	Test_USART_2.pUSART = (struct USART_RegDef_t *) USART6;
	Test_USART_2.USART_Config.USART_Mode = USART_MODE_TX_RX;
	Test_USART_2.USART_Config.USART_DataLength = USART_DATA_LEN_8_BITS;
	Test_USART_2.USART_Config.USART_StopBits = USART_STOP_BITS_1;
	Test_USART_2.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	Test_USART_2.USART_Config.USART_BaudRate = USART_SB_RATE_9600;
	Test_USART_2.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CNTRL_NONE;

	EnablePeriClk(USART6);
	USART_Init(&Test_USART_2);
	USART_PeripheralEnable(&Test_USART_2);

	while(1)
	{
		GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_1);
		USART_ReceiveData(&Test_USART_2,MessageLen,2);
		GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_2);
		USART_ReceiveData(&Test_USART_2,(uint8_t *)message,MessageLen[0]);
		GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_3);
		NumToString(MessageLen[0], MessageLenString);
		USART_SendData(&Test_USART, (uint8_t *)MessageLenStringPrefix, (uint32_t) strlen(MessageLenStringPrefix));
		USART_SendData(&Test_USART, (uint8_t *)MessageLenString, 5);
		USART_SendData(&Test_USART, (uint8_t *)message, (uint32_t) MessageLen[0]);
		delay();
	}

	//USART2 Peripheral Disable
	USART_PeripheralDisable(&Test_USART);
	DisablePeriClk(USART2);

	//USART6 Peripheral Disable
	USART_PeripheralDisable(&Test_USART_2);
	DisablePeriClk(USART6);

	return 0;
}

static void NumToString(uint8_t Num, char *NumString)
{
	char temp;

	if(Num > 99)
	{
		temp = Num/100;
		Num = Num - temp * 100;
	}
	else
	{
		temp = 0;
	}

	*NumString = temp + 48;
	NumString++;

	if(Num > 9)
	{
			temp = Num/10;
			Num = Num - temp * 10;

	}
	else
	{
		temp = 0;
	}

	*NumString = temp + 48;
	NumString++;

	*NumString = Num + 48;
	NumString++;

	*NumString = '\r';
	NumString++;

	*NumString = '\n';

}
