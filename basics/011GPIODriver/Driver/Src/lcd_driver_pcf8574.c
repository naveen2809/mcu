/*
 * lcd_driver_pcf8574.c
 *
 * Driver for I2C LCD Module using PCF8574 8-bit I/0 Expander
 * Reference: https://controllerstech.com/i2c-lcd-in-stm32/
 * Reference: https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller
 *
 *  Created on: 23-Dec-2023
 *      Author: naveen
 */

#include "lcd_driver_pcf8574.h"

extern uint32_t * I2C_DEVICE;
extern uint8_t SlaveAddress;

void lcd_pcf8574_init(void)
{

	//1. Initializing and Configuring the HD44780 display to 4 bit-mode
	delay_us(50000);  			// wait for >40ms
	lcd_pcf8574_send_cmd(0x30);
	delay_us(50000);  			// wait for >4.1ms
	lcd_pcf8574_send_cmd(0x30);
	delay_us(50000);  			// wait for >100us
	lcd_pcf8574_send_cmd(0x30);
	delay_us(50000);
	lcd_pcf8574_send_cmd(0x20);  		// 4bit mode
	delay_us(5000);

	//2. Display Configuration
	lcd_pcf8574_send_cmd(0x28); 	//Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	delay_us(3000);
	lcd_pcf8574_send_cmd(0x08); 	//Display on/off control --> D=0,C=0, B=0  ---> display off
	delay_us(3000);
	lcd_pcf8574_send_cmd(0x01);  	//Clear display
	delay_us(3000);
	lcd_pcf8574_send_cmd(0x02);
	delay_us(3000);
	lcd_pcf8574_send_cmd(0x06); 	//Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	delay_us(3000);
	lcd_pcf8574_send_cmd(0x0C); 	//Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	delay_us(3000);

	return;
}

void lcd_pcf8574_send_cmd(uint8_t cmd)
{
	//Assuming the LCD has been set to 4 bit mode during the initialization
	uint8_t data_u, data_l;
	uint8_t data_t[4];

	data_u = (cmd & 0xF0);
	data_l = ((cmd << 4) & 0xF0);

	data_t[0] = (data_u | 0x0C);
	data_t[1] = (data_u | 0x08);
	data_t[2] = (data_l | 0x0C);
	data_t[3] = (data_l | 0x08);

	I2CMasterSendData(I2C_DEVICE,data_t,4,SlaveAddress);

	return;
}

void lcd_pcf8574_send_data(uint8_t data)
{
	//Assuming the LCD has been set to 4 bit mode during the initialization
	uint8_t data_u, data_l;
	uint8_t data_t[4];

	data_u = (data & 0xF0);
	data_l = ((data << 4) & 0xF0);

	data_t[0] = (data_u | 0x0D);
	data_t[1] = (data_u | 0x09);
	data_t[2] = (data_l | 0x0D);
	data_t[3] = (data_l | 0x09);

	I2CMasterSendData(I2C_DEVICE,data_t,4,SlaveAddress);

	return;
}

void lcd_pcf8574_clear_screen(void)
{
	lcd_pcf8574_send_cmd(0x01);
}

void lcd_pcf8574_return_home(void)
{
	lcd_pcf8574_send_cmd(0x02);
}

void lcd_pcf8574_set_position(uint8_t row, uint8_t col)
{
	uint8_t position, cmd;

	if(row == 0)
	{
		if(col>=0 && col<40)
			position = col;
		else
			position = 0;
	}
	else if(row == 1)
	{
		if(col>=0 && col<40)
			position = 64 + col;
		else
			position = 64;
	}
	else
	{
		position = 0;
	}

	cmd = 0x80 | position;
	lcd_pcf8574_send_cmd(cmd);

	return;
}

void lcd_init(void)
{
	lcd_gpio_init();

	//1. Initializing and Configuring the HD44780 display to 4 bit-mode
	delay_us(50000);  			// wait for >40ms
	lcd_send_cmd(0x30);
	delay_us(5000);  			// wait for >4.1ms
	lcd_send_cmd(0x30);
	delay_us(5000);  			// wait for >100us
	lcd_send_cmd(0x30);
	delay_us(5000);

	//2. Display Configuration
	lcd_send_cmd(0x38); 	//Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	delay_us(30000);
	lcd_send_cmd(0x08); 	//Display on/off control --> D=0,C=0, B=0  ---> display off
	delay_us(30000);
	lcd_send_cmd(0x01);  	//Clear display
	delay_us(30000);
	lcd_send_cmd(0x02);		//Display Home
	delay_us(30000);
	lcd_send_cmd(0x06); 	//Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	delay_us(30000);
	lcd_send_cmd(0x0F); 	//Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	delay_us(30000);

	return;
}

void lcd_gpio_init(void)
{
	EnablePeriClk(LCD_GPIO_PORT);

	GPIOSetMode(LCD_GPIO_PORT,LCD_GPIO_PIN_0,GPIO_MODE_OUTPUT);
	GPIOSetMode(LCD_GPIO_PORT,LCD_GPIO_PIN_1,GPIO_MODE_OUTPUT);
	GPIOSetMode(LCD_GPIO_PORT,LCD_GPIO_PIN_2,GPIO_MODE_OUTPUT);
	GPIOSetMode(LCD_GPIO_PORT,LCD_GPIO_PIN_3,GPIO_MODE_OUTPUT);
	GPIOSetMode(LCD_GPIO_PORT,LCD_GPIO_PIN_4,GPIO_MODE_OUTPUT);
	GPIOSetMode(LCD_GPIO_PORT,LCD_GPIO_PIN_5,GPIO_MODE_OUTPUT);
	GPIOSetMode(LCD_GPIO_PORT,LCD_GPIO_PIN_6,GPIO_MODE_OUTPUT);
	GPIOSetMode(LCD_GPIO_PORT,LCD_GPIO_PIN_7,GPIO_MODE_OUTPUT);

	GPIOWritePin(LCD_GPIO_PORT,LCD_GPIO_PIN_0,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_PORT,LCD_GPIO_PIN_1,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_PORT,LCD_GPIO_PIN_2,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_PORT,LCD_GPIO_PIN_3,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_PORT,LCD_GPIO_PIN_4,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_PORT,LCD_GPIO_PIN_5,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_PORT,LCD_GPIO_PIN_6,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_PORT,LCD_GPIO_PIN_7,GPIO_LOW);

	EnablePeriClk(LCD_GPIO_CONTROL_PORT);
	GPIOSetMode(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_RS,GPIO_MODE_OUTPUT);
	GPIOSetMode(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_RW,GPIO_MODE_OUTPUT);
	GPIOSetMode(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_EN,GPIO_MODE_OUTPUT);

	GPIOWritePin(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_RS,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_RW,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_EN,GPIO_LOW);

	return;
}

void lcd_send_cmd(uint8_t cmd)
{
	//GPIOWritePort(LCD_GPIO_PORT,(uint32_t)cmd);
	GPIOWritePort(LCD_GPIO_PORT,(uint32_t)cmd);
	GPIOWritePin(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_RS,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_RW,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_EN,GPIO_HIGH);
	delay_us(10);
	GPIOWritePin(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_EN,GPIO_LOW);

	return;
}

void lcd_send_data(uint8_t data)
{

	GPIOWritePort(LCD_GPIO_PORT,(uint32_t)data);
	GPIOWritePin(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_RS,GPIO_HIGH);
	GPIOWritePin(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_RW,GPIO_LOW);
	GPIOWritePin(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_EN,GPIO_HIGH);
	delay_us(10);
	GPIOWritePin(LCD_GPIO_CONTROL_PORT,LCD_GPIO_PIN_EN,GPIO_LOW);

	return;
}
