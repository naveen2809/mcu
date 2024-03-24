/*
 * common_utils.c
 *
 *  Created on: 24-Dec-2023
 *      Author: naveen
 */

#include "common_utils.h"

struct General_Purpose_Timer_Handle_t TIM2_Handle;
struct USART_Handle_t Test_USART;

void delay_us(uint32_t delay)
{
	TIM2_Handle.pGeneral_Purpose_Timer->TIMx_CNT = 0;
	while(TIM2_Handle.pGeneral_Purpose_Timer->TIMx_CNT < 16*delay);
}

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

void configure_delay_timer(void)
{
	//TIM2 Configuration
	TIM2_Handle.pGeneral_Purpose_Timer = (struct General_Purpose_Timer_RegDef_t *) TIM2;
	memset(&TIM2_Handle.General_Purpose_Timer_Config,0,sizeof(TIM2_Handle.General_Purpose_Timer_Config));
	TIM2_Handle.General_Purpose_Timer_Config.Timer_PreScalerValue = 0x0;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_AutoReloadValue = 0xFFFFFFFF;

	EnablePeriClk(TIM2);
	General_Purpose_Timer_Init(&TIM2_Handle);
	General_Purpose_Timer_PeripheralEnable(&TIM2_Handle);
}

void configure_uart(void)
{
	//GPIO Pin Configuration
	EnablePeriClk(UART_GPIO_PORT);
	GPIOSetMode(UART_GPIO_PORT,UART_TX_PIN,GPIO_MODE_ALTFN);
	GPIOSetMode(UART_GPIO_PORT,UART_RX_PIN,GPIO_MODE_ALTFN);
	GPIOSetAltFn(UART_GPIO_PORT,UART_TX_PIN,GPIO_ALTFN_8);
	GPIOSetAltFn(UART_GPIO_PORT,UART_RX_PIN,GPIO_ALTFN_8);
	GPIOSetOutputType(UART_GPIO_PORT,UART_TX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputType(UART_GPIO_PORT,UART_RX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputSpeed(UART_GPIO_PORT,UART_TX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetOutputSpeed(UART_GPIO_PORT,UART_RX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(UART_GPIO_PORT,UART_TX_PIN,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(UART_GPIO_PORT,UART_RX_PIN,GPIO_PULL_UP);

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
}

void UART_SendChar(uint8_t ch)
{
	USART_SendData(&Test_USART, &ch, 1);
}
