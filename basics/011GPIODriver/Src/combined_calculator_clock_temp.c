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
#include "general_purpose_timer.h"
#include "rtc_driver.h"
#include "ds3231_rtc_driver.h"
#include "dht11_driver.h"
#include "common_utils.h"
#include "external_interrupt_config.h"

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

#define GPIO_ROW_1_IRQ_NUM 6
#define GPIO_ROW_2_IRQ_NUM 7
#define GPIO_ROW_3_IRQ_NUM 8
#define GPIO_ROW_4_IRQ_NUM 9

static uint8_t gpio_col[] = {GPIO_COL_1, GPIO_COL_2, GPIO_COL_3, GPIO_COL_4};
static uint8_t gpio_row[] = {GPIO_ROW_1, GPIO_ROW_2, GPIO_ROW_3, GPIO_ROW_4};

static uint8_t char_matrix[4][4] = {
							{'1','2','3','A'},
							{'4','5','6','B'},
							{'7','8','9','C'},
							{'*','0','#','D'}
						};

static uint8_t char_buffer[80];
static uint8_t char_count = 0;
static uint8_t c, clock_mode = 1, calculator_mode = 0, temp_mode = 0;

struct Date date;
struct Time time;
extern uint32_t ds3231_display_count;

static int calculation(uint8_t *buffer, uint8_t count);
static void process_key_press(void);
static void update_clock(void);
static void update_temp(void);
static void disable_interrupts(void);
static void enable_interrupts(void);

int main(void)
{

	struct RTC_DS3231_Config_t ds3231_rtc_config;

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


	lcd_pcf8574_clear_screen();
	delay_us(3000);
	lcd_pcf8574_return_home();
	delay_us(3000);
	//printf(">");

	//GPIO Configuration for Columns of Matrix Keypad
	EnablePeriClk(GPIO_PORT_COL);
	GPIOSetMode(GPIO_PORT_COL, GPIO_COL_1, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIO_PORT_COL, GPIO_COL_2, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIO_PORT_COL, GPIO_COL_3, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIO_PORT_COL, GPIO_COL_4, GPIO_MODE_OUTPUT);

	GPIOWritePin(GPIO_PORT_COL, GPIO_COL_1,GPIO_LOW);
	GPIOWritePin(GPIO_PORT_COL, GPIO_COL_2,GPIO_LOW);
	GPIOWritePin(GPIO_PORT_COL, GPIO_COL_3,GPIO_LOW);
	GPIOWritePin(GPIO_PORT_COL, GPIO_COL_4,GPIO_LOW);

	//GPIO Configuration for Rows of Matrix Keypad
	configure_external_gpio_interrupt(GPIO_PORT_ROW,GPIO_ROW_1,EXTI_FALLING_TRIGGER,GPIO_ROW_1_IRQ_NUM);
	configure_external_gpio_interrupt(GPIO_PORT_ROW,GPIO_ROW_2,EXTI_FALLING_TRIGGER,GPIO_ROW_2_IRQ_NUM);
	configure_external_gpio_interrupt(GPIO_PORT_ROW,GPIO_ROW_3,EXTI_FALLING_TRIGGER,GPIO_ROW_3_IRQ_NUM);
	configure_external_gpio_interrupt(GPIO_PORT_ROW,GPIO_ROW_4,EXTI_FALLING_TRIGGER,GPIO_ROW_4_IRQ_NUM);

	//Configure DS3231 RTC Peripheral
	//a. Configure the I2C Port for the RTC
	configure_i2c_rtc();

	//b. Configure the parameters of the RTC
	memset(&ds3231_rtc_config,0,sizeof(ds3231_rtc_config));
	ds3231_rtc_config.RTCClockSecondsUnits = 0;
	ds3231_rtc_config.RTCClockSecondsTens = 0;
	ds3231_rtc_config.RTCClockMinutesUnits = 8;
	ds3231_rtc_config.RTCClockMinutesTens = 2;
	ds3231_rtc_config.RTCClockHoursUnits = 1;
	ds3231_rtc_config.RTCClockHoursTens = 1;
	ds3231_rtc_config.RTCClockHourFormat = DS3231_RTC_HOUR_FORMAT_12;
	ds3231_rtc_config.RTCClockAMPM = DS3231_RTC_HOUR_AM;
	ds3231_rtc_config.RTCClockDateUnits = 7;
	ds3231_rtc_config.RTCClockDateTens = 1;
	ds3231_rtc_config.RTCClockMonthUnits = 1;
	ds3231_rtc_config.RTCClockMonthTens = 0;
	ds3231_rtc_config.RTCClockYearUnits = 4;
	ds3231_rtc_config.RTCClockYearTens = 2;
	ds3231_rtc_config.RTCClockDayOfWeek = RTC_CLK_DOW_THU;

	RTC_DS3231_Config_Calendar(&ds3231_rtc_config);
	delay_us(10000);

	while(1)
	{
		if(clock_mode == 1)
		{
			disable_interrupts();
			update_clock();
			enable_interrupts();
		}
		else if(temp_mode == 1)
		{
			disable_interrupts();
			update_temp();
			enable_interrupts();
		}
	}

	return 0;
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

void EXTI0_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;
	uint8_t row,col;

	row=0;
	//Handling the Interrupt
	for(col=0;col<4;col++)
	{
		GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_HIGH);
		if(GPIOReadPin(GPIO_PORT_ROW, gpio_row[row]) == GPIO_HIGH)
		{
			c = char_matrix[row][col];

			//Disabling the Interrupt
			//NVIC_DisableIRQ(GPIO_ROW_1_IRQ_NUM);

			GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_LOW);

			break;
		}
		GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_LOW);
	}

	//printf("%c\r\n",c);

	delay_us(500000);

	process_key_press();

	//Clearing the Button Interrupt
	*pEXTI_PR |= (1 << GPIO_ROW_1);		// Clearing the EXTI_PR Register

	//Re-Enabling the Interrupt
	//NVIC_EnableIRQ(GPIO_ROW_1_IRQ_NUM);

	return;
}

void EXTI1_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;
	uint8_t row,col;

	row=1;
	//Handling the Interrupt
	for(col=0;col<4;col++)
	{
		GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_HIGH);
		if(GPIOReadPin(GPIO_PORT_ROW, gpio_row[row]) == GPIO_HIGH)
		{
			c = char_matrix[row][col];

			//Disabling the Interrupt
			//NVIC_DisableIRQ(GPIO_ROW_2_IRQ_NUM);

			GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_LOW);

			break;
		}
		GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_LOW);
	}

	//printf("%c\r\n",c);

	delay_us(500000);

	process_key_press();

	//Clearing the Button Interrupt
	*pEXTI_PR |= (1 << GPIO_ROW_2);		// Clearing the EXTI_PR Register

	//Re-Enabling the Interrupt
	//NVIC_EnableIRQ(GPIO_ROW_2_IRQ_NUM);

	return;
}

void EXTI2_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;
	uint8_t row,col;

	row=2;
	//Handling the Interrupt
	for(col=0;col<4;col++)
	{
		GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_HIGH);
		if(GPIOReadPin(GPIO_PORT_ROW, gpio_row[row]) == GPIO_HIGH)
		{
			c = char_matrix[row][col];

			//Disabling the Interrupt
			//NVIC_DisableIRQ(GPIO_ROW_3_IRQ_NUM);

			GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_LOW);

			break;
		}
		GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_LOW);
	}

	//printf("%c\r\n",c);

	delay_us(500000);

	process_key_press();
	//Clearing the Button Interrupt
	*pEXTI_PR |= (1 << GPIO_ROW_3);		// Clearing the EXTI_PR Register

	//Re-Enabling the Interrupt
	//NVIC_EnableIRQ(GPIO_ROW_3_IRQ_NUM);

	return;
}

void EXTI3_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;
	uint8_t row,col;

	row=3;
	//Handling the Interrupt
	for(col=0;col<4;col++)
	{
		GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_HIGH);
		if(GPIOReadPin(GPIO_PORT_ROW, gpio_row[row]) == GPIO_HIGH)
		{
			c = char_matrix[row][col];

			//Disabling the Interrupt
			//NVIC_DisableIRQ(GPIO_ROW_4_IRQ_NUM);

			GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_LOW);

			break;
		}
		GPIOWritePin(GPIO_PORT_COL, gpio_col[col],GPIO_LOW);
	}

	//printf("%c\r\n",c);

	delay_us(500000);

	process_key_press();

	//Clearing the Button Interrupt
	*pEXTI_PR |= (1 << GPIO_ROW_4);		// Clearing the EXTI_PR Register

	//Re-Enabling the Interrupt
	//NVIC_EnableIRQ(GPIO_ROW_4_IRQ_NUM);

	return;
}

static void process_key_press(void)
{
	uint8_t printed_c;
	int value;

	if(calculator_mode == 1)
	{
		char_buffer[char_count] = c;
		char_count++;

		if(c>=48 && c<=57)
		{
			printed_c = c;
			printf("%c", printed_c);
			return;
		}
		else
		{
			if(c == 'A')
			{
				if(char_count != 1)
				{
					printed_c = '+';
					printf("%c", printed_c);
					return;
				}
				else if(char_count == 1)
				{
					calculator_mode = 0;
					clock_mode = 0;
					temp_mode = 1;
					return;
				}
			}
			else if(c == 'B')
			{
				printed_c = '-';
				printf("%c", printed_c);
				return;
			}
			else if(c == 'C')
			{
				if(char_count != 1)
				{
					printed_c = 'x';
					printf("%c", printed_c);
					return;
				}
				else if(char_count == 1)
				{
					calculator_mode = 0;
					clock_mode = 1;
					temp_mode = 0;
					ds3231_display_count = 0;
				}
			}
			else if(c == 'D')
			{
				printed_c = '/';
				printf("%c", printed_c);
				return;
			}
			else if(c == '#')
			{
				printed_c = '=';
				printf("%c", printed_c);
				value = calculation(char_buffer,char_count);
				printf("%d", value);
				char_count = 0;
				return;
			}
			else if(c == '*')
			{
				char_count = 0;
				lcd_pcf8574_clear_screen();
				delay_us(3000);
				lcd_pcf8574_return_home();
				delay_us(3000);
				printf(">");
				return;
			}
		}
	}
	else
	{
		if(c == '*')
		{
			calculator_mode = 1;
			clock_mode = 0;
			temp_mode = 0;
			char_count = 0;
			lcd_pcf8574_clear_screen();
			delay_us(3000);
			lcd_pcf8574_return_home();
			delay_us(3000);
			printf(">");
			return;
		}
		else if(c == 'A')
		{
			calculator_mode = 0;
			clock_mode = 0;
			temp_mode = 1;
			return;
		}
		else if(c == 'C')
		{
			calculator_mode = 0;
			clock_mode = 1;
			temp_mode = 0;
			ds3231_display_count = 0;
			return;
		}
	}
}

static void update_clock(void)
{

	if(clock_mode == 1)
	{
		RTC_DS3231_Read_Calendar(&date,&time);
		RTC_DS3231_Display_Calendar_LCD(&date,&time);
	}

	return;
}

static void update_temp(void)
{
	//DHT11 Sensor Related Variables
	uint8_t dht11_sensor_response;
	uint8_t dht11_sensor_data[5];

	if(temp_mode == 1)
	{
		dht11_sensor_response = read_dht11_sensor(dht11_sensor_data);

		if(dht11_sensor_response)
		{
			//Ensure that printf() is redirected to LCD in "syscalls.c" file
			//Print RH Data
			lcd_pcf8574_clear_screen();
			delay_us(3000);
			lcd_pcf8574_return_home();
			delay_us(3000);
			printf("RH:   %2d.%2d",dht11_sensor_data[0],dht11_sensor_data[1]);

			//Print TEMP Data
			lcd_pcf8574_set_position(1,0);
			delay_us(3000);
			printf("TEMP: %2d.%2d C",dht11_sensor_data[2],dht11_sensor_data[3]);
		}
		else
		{
			lcd_pcf8574_clear_screen();
			delay_us(3000);
			lcd_pcf8574_return_home();
			delay_us(3000);
			printf("Sensor Read Failed");
		}

		delay_us(1000000);
	}

	return;
}

static void disable_interrupts(void)
{
	__asm volatile ("CPSID I");

	return;
}

static void enable_interrupts(void)
{
	__asm volatile ("CPSIE I");

	return;
}