/*
 * temp_on_lcd_over_wireless_rx.c
 * Displaying the RH and Temperature data received over nRFL01 Wireless Link
 * Reference: dht11_sensor_driver.c and lcd_pcf8574_test.c
 *  Created on: 24-Dec-2023
 *      Author: naveen
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "common_utils.h"
#include "external_interrupt_config.h"

#define BTN_INT_GPIO_PORT		GPIOD
#define BTN_INT_GPIO_PIN		GPIO_PIN_0
#define BTN_INT_IRQ_NUM			6

#define USART2_IRQ_NUM	38

extern struct USART_Handle_t Test_USART;

void USART_RX_Interrupt_Application_Callback(uint8_t data);
extern void ITM_SendChar(uint8_t ch);

char message_isr[] = "This message is from Button ISR\r\n";

int main(void)
{
	char message1[] = "Message 1\r\n";
	char message[] = "Testing Optimized and Updated UART Interrupt Driver With Synchronization Support: Hello World!!!\r\n";

	//Configure the Timer
	configure_delay_timer();

	//Configure UART
	configure_uart();

	//Configure the Button Interrupt
	configure_external_gpio_interrupt(BTN_INT_GPIO_PORT,BTN_INT_GPIO_PIN,EXTI_FALLING_TRIGGER,BTN_INT_IRQ_NUM);

	//Enable the Receive Interrupt
	USART_EnableRXNEInterrupt(&Test_USART);

	//Enable the UART Interrupt
	USART_SendData_Interrupt(message1,strlen(message1),USART2_IRQ_NUM);
	USART_SendData_Interrupt(message,strlen(message),USART2_IRQ_NUM);



	while(1)
	{

	}

	return 0;
}

void USART2_IRQHandler(void)
{

	if(USART_GetFlagStatus(&Test_USART,USART_FLAG_TXE))
	{
		USART_Tx_Interrupt_Callback(&Test_USART);
	}

	if(USART_GetFlagStatus(&Test_USART,USART_FLAG_RXNE))
	{
		USART_Rx_Interrupt_Callback(&Test_USART);
	}

	return;
}

void USART_RX_Interrupt_Application_Callback(uint8_t data)
{
	ITM_SendChar(data);
}

void EXTI0_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	//Handling the Interrupt
	USART_SendData_Interrupt(message_isr,strlen(message_isr),USART2_IRQ_NUM);

	//De-bouncing the switch
	delay_us(500000);

	//Clearing the Button Interrupt
	*pEXTI_PR |= (1 << BTN_INT_GPIO_PIN);		// Clearing the EXTI_PR Register

	return;
}
