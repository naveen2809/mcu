/*
 * lcd_driver_pcf8574.h
 *
 * Code for testing the I2C LCD Driver using PCF8574 8-bit I/0 Expander
 *  Reference: https://controllerstech.com/i2c-lcd-in-stm32/
 *  Reference: https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller
 *  Created on: 23-Dec-2023
 *      Author: naveen
 */

#ifndef INC_LCD_DRIVER_PCF8574_H_
#define INC_LCD_DRIVER_PCF8574_H_

#include "stm32f4xx.h"
#include "i2c_driver.h"
#include "gpio_driver.h"
#include "common_utilities.h"

#define LCD_GPIO_PORT 				GPIOA
#define LCD_GPIO_CONTROL_PORT		GPIOC
#define LCD_GPIO_PIN_0 				GPIO_PIN_0
#define LCD_GPIO_PIN_1 				GPIO_PIN_1
#define LCD_GPIO_PIN_2 				GPIO_PIN_2
#define LCD_GPIO_PIN_3 				GPIO_PIN_3
#define LCD_GPIO_PIN_4 				GPIO_PIN_4
#define LCD_GPIO_PIN_5 				GPIO_PIN_5
#define LCD_GPIO_PIN_6 				GPIO_PIN_6
#define LCD_GPIO_PIN_7 				GPIO_PIN_7
#define LCD_GPIO_PIN_RS 			GPIO_PIN_0
#define LCD_GPIO_PIN_RW 			GPIO_PIN_1
#define LCD_GPIO_PIN_EN				GPIO_PIN_2


void lcd_pcf8574_init(void);
void lcd_pcf8574_send_cmd(uint8_t cmd);
void lcd_pcf8574_send_data(uint8_t data);
void lcd_pcf8574_clear_screen(void);
void lcd_pcf8574_return_home(void);
void lcd_pcf8574_set_position(uint8_t row, uint8_t col);

void lcd_init(void);
void lcd_gpio_init(void);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);

#endif /* INC_LCD_DRIVER_PCF8574_H_ */
