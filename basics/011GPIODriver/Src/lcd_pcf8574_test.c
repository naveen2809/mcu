/*
 * lcd_driver_pcf8574.c
 * Code for testing the I2C LCD Driver using PCF8574 8-bit I/0 Expander
 * Reference: https://controllerstech.com/i2c-lcd-in-stm32/
 * Reference: https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller
 * Created on: 23-Dec-2023
 * Author: naveen
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "i2c_driver.h"
#include "gpio_driver.h"
#include "lcd_driver_pcf8574.h"
#include "common_utils.h"

int main(void)
{
	uint16_t i;
	char message[]="Hello World!!!";
	char message2[]="Brave New World!";

	//1a. Configure the I2C interface
	configure_i2c();

	//1.b Configure the Timer Peripheral
	configure_delay_timer();

	setbuf(stdout, NULL);

	//2. Initialize the LDC module
	delay_us(1000000);
	lcd_pcf8574_init();

	//3. Send message to the LCD module
	for(i=0;i<strlen(message);i++)
	{
		lcd_pcf8574_send_data((uint8_t)message[i]);
	}

	lcd_pcf8574_send_cmd(0xC0);

	//3. Send message to the LCD module
	for(i=0;i<strlen(message2);i++)
	{
		lcd_pcf8574_send_data((uint8_t)message2[i]);
	}

	delay_us(5000000);

	lcd_pcf8574_clear_screen();
	delay_us(3000);
	lcd_pcf8574_return_home();
	delay_us(3000);

	lcd_pcf8574_set_position(0,0);
	printf("%c",'H');
	lcd_pcf8574_set_position(0,2);
	printf("%c",'E');
	lcd_pcf8574_set_position(0,4);
	printf("%c",'L');
	lcd_pcf8574_set_position(0,6);
	printf("%c",'L');
	lcd_pcf8574_set_position(0,8);
	printf("%c",'O');

	lcd_pcf8574_set_position(1,0);
	printf("%c",'W');
	lcd_pcf8574_set_position(1,2);
	printf("%c",'O');
	lcd_pcf8574_set_position(1,4);
	printf("%c",'R');
	lcd_pcf8574_set_position(1,6);
	printf("%c",'L');
	lcd_pcf8574_set_position(1,8);
	printf("%c",'D');

	delay_us(5000000);

	for(i=0;i<=1000;i++)
	{
		lcd_pcf8574_clear_screen();
		delay_us(3000);
		lcd_pcf8574_return_home();
		delay_us(3000);
		printf("Counting Up:%d",i);
		delay_us(1000000);
	}


	while(1);

	return 0;
}
