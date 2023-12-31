/*
 * calculator_matrix_keypad.c
 * Interfaced 4x4 matrix keypad with MCU and developed a small calculator application based on it
 * Matrix Keypad physical connections:
 * First 4 pins (Pin0-Pin3) are connected to Rows and next 4 pins (Pin4-Pin7) are connected to Columns
 * Reference: https://lastminuteengineers.com/arduino-keypad-tutorial/
 *  Created on: 17-Dec-2023
 *      Author: naveen
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "usart_driver.h"
#include "general_purpose_timer.h"
#include "common_utils.h"

#define GPIO_COL_1		GPIO_PIN_0
#define GPIO_COL_2		GPIO_PIN_1
#define GPIO_COL_3		GPIO_PIN_2
#define GPIO_COL_4		GPIO_PIN_3

#define GPIO_ROW_1		GPIO_PIN_0
#define GPIO_ROW_2		GPIO_PIN_1
#define GPIO_ROW_3		GPIO_PIN_2
#define GPIO_ROW_4		GPIO_PIN_3

#define GPIO_PORT_COL	GPIOC
#define GPIO_PORT_ROW	GPIOA

#define UART_GPIO_PORT		GPIOD
#define UART_TX_PIN			GPIO_PIN_5
#define UART_RX_PIN			GPIO_PIN_6

uint8_t gpio_col[] = {GPIO_COL_1, GPIO_COL_2, GPIO_COL_3, GPIO_COL_4};
uint8_t gpio_row[] = {GPIO_ROW_1, GPIO_ROW_2, GPIO_ROW_3, GPIO_ROW_4};

uint8_t char_matrix[4][4] = {
							{'1','2','3','A'},
							{'4','5','6','B'},
							{'7','8','9','C'},
							{'*','0','#','D'}
						};

uint8_t char_buffer[80];
uint8_t char_count = 0;

uint8_t getchar_keypad(void);
int calculation(uint8_t *buffer, uint8_t count);

int main(void)
{

	uint8_t c, printed_c;
	int value;

	//Configure the Timer
	configure_delay_timer();

	//Configure the I2C Peripheral
	configure_i2c();

	//Disable printf() buffering
	setbuf(stdout, NULL);

	//Initialize the LCD module
	delay_us(1000000);
	lcd_pcf8574_init();
	delay_us(100000);

	//GPIO Configuration for Columns of Matrix Keypad
	EnablePeriClk(GPIO_PORT_COL);
	GPIOSetMode(GPIO_PORT_COL, GPIO_COL_1, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIO_PORT_COL, GPIO_COL_2, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIO_PORT_COL, GPIO_COL_3, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIO_PORT_COL, GPIO_COL_4, GPIO_MODE_OUTPUT);

	GPIOWritePin(GPIO_PORT_COL, GPIO_COL_1,GPIO_HIGH);
	GPIOWritePin(GPIO_PORT_COL, GPIO_COL_2,GPIO_HIGH);
	GPIOWritePin(GPIO_PORT_COL, GPIO_COL_3,GPIO_HIGH);
	GPIOWritePin(GPIO_PORT_COL, GPIO_COL_4,GPIO_HIGH);

	//GPIO Configuration for Rows of Matrix Keypad
	EnablePeriClk(GPIO_PORT_ROW);
	GPIOSetMode(GPIO_PORT_ROW, GPIO_ROW_1, GPIO_MODE_INPUT);
	GPIOSetMode(GPIO_PORT_ROW, GPIO_ROW_2, GPIO_MODE_INPUT);
	GPIOSetMode(GPIO_PORT_ROW, GPIO_ROW_3, GPIO_MODE_INPUT);
	GPIOSetMode(GPIO_PORT_ROW, GPIO_ROW_4, GPIO_MODE_INPUT);

	GPIOSetPullUpDownConfig(GPIO_PORT_ROW, GPIO_ROW_1,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(GPIO_PORT_ROW, GPIO_ROW_2,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(GPIO_PORT_ROW, GPIO_ROW_3,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(GPIO_PORT_ROW, GPIO_ROW_4,GPIO_PULL_UP);

	while(1)
	{
		char_count = 0;
		lcd_pcf8574_clear_screen();
		delay_us(3000);
		lcd_pcf8574_return_home();
		delay_us(3000);
		printf(">");
		do
		{
			c = getchar_keypad();
			char_buffer[char_count] = c;
			char_count++;

			if(c>=48 && c<=57)
			{
				printed_c = c;
			}
			else
			{
				if(c == 'A')
					printed_c = '+';
				else if(c == 'B')
					printed_c = '-';
				else if(c == 'C')
					printed_c = 'x';
				else if(c == 'D')
					printed_c = '/';
				else if(c == '#')
					printed_c = '=';
				else if(c == '*')
				{
					delay_us(300000);
					break;
				}
			}

			printf("%c", printed_c);
			delay_us(300000);
		}
		while(c != '#');

		if(c != '*')
		{
			value = calculation(char_buffer,char_count);
			printf("%d", value);

			while(getchar_keypad() != '*');
		}
	}

	return 0;
}

uint8_t getchar_keypad(void)
{
	uint8_t row,col;
	uint8_t c;

	while(1)
	{
		for(row=0;row<4;row++)
		{
			for(col=0;col<4;col++)
			{
				GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_LOW);
				if(GPIOReadPin(GPIO_PORT_ROW, gpio_row[row]) == GPIO_LOW)
				{
					c = char_matrix[row][col];
					GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_HIGH);
					return c;
				}
				GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_HIGH);
			}
		}
	}
}

int calculation(uint8_t *buffer, uint8_t count)
{
	int num1,num2,value,i,j,k,place_value;
	uint8_t operation;

	i=0;
	//Parsing the first number
	while(buffer[i]>=48 && buffer[i] <=57)
		i++;

	operation = buffer[i];

	num1 = 0;
	for(j=i-1;j>=0;j--)
	{
		place_value = 1;
		for(k=i-1;k>j;k--)
			place_value *= 10;
		num1 += (buffer[j]-48)*place_value;
	}

	//Parsing the second number
	num2 = 0;
	for(j=count-2;j>i;j--)
	{
			place_value = 1;
			for(k=count-2;k>j;k--)
				place_value *= 10;
			num2 += (buffer[j]-48)*place_value;
	}

	switch(operation)
	{
		case 'A':
			value = num1 + num2;
			break;
		case 'B':
			value = num1 - num2;
			break;
		case 'C':
			value = num1 * num2;
			break;
		case 'D':
			value = num1 / num2;
			break;
		default:
			break;
	}

	return value;

}
