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
#include "common_utilities.h"

#define I2C_CONFIG_PERI_FREQ			0x10
#define I2C_CONFIG_CCR_VALUE			0x50
#define TRISE_VALUE						0x11
#define I2C_PORT						GPIOB
#define I2C1_SDA						GPIO_PIN_7
#define I2C1_SCL						GPIO_PIN_6

void configure_i2c(void);

uint32_t * I2C_DEVICE = I2C1;
uint8_t SlaveAddress = 0x27;

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

	while(1);

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

void configure_i2c(void)
{
	struct I2C_Config_t I2C_Config;

	memset(&I2C_Config,0,sizeof(I2C_Config));

	//GPIO Pin Configuration for I2C
	EnablePeriClk(I2C_PORT);
	GPIOSetMode(I2C_PORT,I2C1_SDA,GPIO_MODE_ALTFN);
	GPIOSetMode(I2C_PORT,I2C1_SCL,GPIO_MODE_ALTFN);
	GPIOSetAltFn(I2C_PORT,I2C1_SDA,GPIO_ALTFN_4);
	GPIOSetAltFn(I2C_PORT,I2C1_SCL,GPIO_ALTFN_4);
	GPIOSetOutputType(I2C_PORT,I2C1_SDA,GPIO_OPTYPE_OD);
	GPIOSetOutputType(I2C_PORT,I2C1_SCL,GPIO_OPTYPE_OD);
	GPIOSetOutputSpeed(I2C_PORT,I2C1_SDA,GPIO_OPSPEED_HIGH);
	GPIOSetOutputSpeed(I2C_PORT,I2C1_SCL,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(I2C_PORT,I2C1_SDA,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(I2C_PORT,I2C1_SCL,GPIO_PULL_UP);

	//I2C Port Configuration
	I2C_Config.I2C_PeriFreq = I2C_CONFIG_PERI_FREQ;
	I2C_Config.I2C_SpeedMode = I2C_MODE_SM;
	I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_0;
	I2C_Config.I2C_CCRValue = I2C_CONFIG_CCR_VALUE;
	I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;

	EnablePeriClk(I2C_DEVICE);

	I2CPeriConfig(I2C_DEVICE,&I2C_Config);
	I2CConfigureTrise(I2C_DEVICE,TRISE_VALUE);
	I2CEnable(I2C_DEVICE);

	return;
}

void LCD_SendChar(uint8_t ch)
{
	lcd_pcf8574_send_data(ch);
}

