/*
 * temp_on_lcd.c
 * Displaying the RH and Temperature data from DHT11 Sensor on I2C LCD Module
 * Reference: dht11_sensor_driver.c and lcd_pcf8574_test.c
 *  Created on: 24-Dec-2023
 *      Author: naveen
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "i2c_driver.h"
#include "general_purpose_timer.h"
#include "lcd_driver_pcf8574.h"
#include "dht11_driver.h"
#include "common_utils.h"

#define TIMER5_IRQ_NUM	50

static struct General_Purpose_Timer_Handle_t TIM5_Handle;

static void initialize_temp_display(void);
static void configure_timer5(void);

int main(void)
{
	//Configure the Timer Module for delay_us function
	configure_delay_timer();

	//Configure the I2C Peripheral
	configure_i2c();

	//Disable printf() buffering
	setbuf(stdout, NULL);

	//Initialize the LCD module
	delay_us(1000000);
	lcd_pcf8574_init();
	delay_us(100000);

	initialize_temp_display();

	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOD,GPIO_PIN_0,GPIO_MODE_OUTPUT);
	GPIOWritePin(GPIOD,GPIO_PIN_0,GPIO_LOW);

	while(1)
	{

	}

	return 0;
}

static void initialize_temp_display(void)
{
	//DHT11 Sensor Related Variables
	uint8_t dht11_sensor_response;
	uint8_t dht11_sensor_data[5];

	//Display the temperature for the first time
	//Read the DHT11 Sensor
	dht11_sensor_response = read_dht11_sensor(dht11_sensor_data);

	if(dht11_sensor_response)
	{
		//Ensure that printf() is redirected to LCD in "syscalls.c" file
		//Print RH Data
		lcd_pcf8574_clear_screen();
		delay_us(3000);
		lcd_pcf8574_return_home();
		delay_us(3000);
		printf("RH:   %2d.%d %%",dht11_sensor_data[0],dht11_sensor_data[1]);

		//Print TEMP Data
		lcd_pcf8574_set_position(1,0);
		delay_us(3000);
		printf("TEMP: %2d.%d C",dht11_sensor_data[2],dht11_sensor_data[3]);
	}
	else
	{
		lcd_pcf8574_clear_screen();
		delay_us(3000);
		lcd_pcf8574_return_home();
		delay_us(3000);
		printf("Sensor Read Failed");
	}

	//Initialize the Timer Interrupts
	configure_timer5();

	return;
}

static void configure_timer5(void)
{
	TIM5_Handle.pGeneral_Purpose_Timer = (struct General_Purpose_Timer_RegDef_t *) TIM5;
	memset(&TIM5_Handle.General_Purpose_Timer_Config,0,sizeof(TIM5_Handle.General_Purpose_Timer_Config));
	TIM5_Handle.General_Purpose_Timer_Config.Timer_PreScalerValue = 0x0;
	TIM5_Handle.General_Purpose_Timer_Config.Timer_AutoReloadValue = 0x4C4B400;
	TIM5_Handle.General_Purpose_Timer_Config.Timer_UpdateInterruptConfig = TIMER_UPDATE_INTERRUPT_ENABLE;

	EnablePeriClk(TIM5);
	General_Purpose_Timer_Init(&TIM5_Handle);
	General_Purpose_Timer_PeripheralEnable(&TIM5_Handle);

	NVIC_EnableIRQ(TIMER5_IRQ_NUM);
}

void TIM5_IRQHandler(void)
{
	//DHT11 Sensor Related Variables
	uint8_t dht11_sensor_response;
	uint8_t dht11_sensor_data[5];

	//Toggle GPIO
	GPIOTogglePin(GPIOD,GPIO_PIN_0);

	//Display the temperature for the first time
	//Read the DHT11 Sensor
	dht11_sensor_response = read_dht11_sensor(dht11_sensor_data);

	if(dht11_sensor_response)
	{
		//Ensure that printf() is redirected to LCD in "syscalls.c" file
		//Print RH Data
		lcd_pcf8574_clear_screen();
		delay_us(3000);
		lcd_pcf8574_return_home();
		delay_us(3000);
		printf("RH:   %2d.%d %%",dht11_sensor_data[0],dht11_sensor_data[1]);

		//Print TEMP Data
		lcd_pcf8574_set_position(1,0);
		delay_us(3000);
		printf("TEMP: %2d.%d C",dht11_sensor_data[2],dht11_sensor_data[3]);
	}
	else
	{
		lcd_pcf8574_clear_screen();
		delay_us(3000);
		lcd_pcf8574_return_home();
		delay_us(3000);
		printf("Sensor Read Failed");
	}

	//Clear the TIM5 Interrupt
	General_Purpose_Timer_ClearUpdateEventFlag(&TIM5_Handle);

	return;
}
