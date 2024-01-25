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

#define USART2_IRQ_NUM	38

extern struct USART_Handle_t Test_USART;

int main(void)
{
	char message1[] = "Message 1\r\n";
	char message[] = "Testing Optimized UART Interrupt Driver With Synchronization Support: Hello World!!!\r\n";

	//Configure the Timer
	configure_delay_timer();

	//Configure UART
	configure_uart();

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
	USART_Interrupt_Callback(&Test_USART);

	return;
}