/*
 * common_utils.h
 *
 *  Created on: 24-Dec-2023
 *      Author: naveen
 */

#ifndef INC_COMMON_UTILS_H_
#define INC_COMMON_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "i2c_driver.h"
#include "spi_driver.h"
#include "usart_driver.h"
#include "timer_driver.h"
#include "general_purpose_timer.h"
#include "lcd_driver_pcf8574.h"

#define I2C_CONFIG_PERI_FREQ			0x10
#define I2C_CONFIG_CCR_VALUE			0x50
#define TRISE_VALUE						0x11
#define I2C_PORT						GPIOB
#define I2C1_SDA						GPIO_PIN_7
#define I2C1_SCL						GPIO_PIN_6

#define SPI_DEV			SPI2
#define SPI_PORT		GPIOB
#define SPI_PIN_MOSI	15
#define SPI_PIN_MISO	14
#define SPI_PIN_CLK		13
#define SPI_PIN_SS		12

#define UART_GPIO_PORT		GPIOD
#define UART_TX_PIN			GPIO_PIN_5
#define UART_RX_PIN			GPIO_PIN_6

void delay_us(uint32_t delay);
void configure_delay_timer(void);
void configure_i2c(void);
void configure_spi(void);
void configure_uart(void);
void UART_SendChar(uint8_t ch);
void LCD_SendChar(uint8_t ch);

#endif /* INC_COMMON_UTILS_H_ */
