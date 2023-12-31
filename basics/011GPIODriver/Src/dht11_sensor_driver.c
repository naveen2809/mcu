/*
 * dht11_sensor_driver.c
 * Reference: DHT11 Sensor Driver and Test Program
 * Reference: https://controllerstech.com/using-dht11-sensor-with-stm32/
 *  Created on: 13-Dec-2023
 *      Author: naveen
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "general_purpose_timer.h"
#include "usart_driver.h"
#include "common_utilities.h"

#define DHT11_SENSOR_DATA_PIN		GPIO_PIN_0
#define DHT11_SENSOR_PORT			GPIOA

#define UART_GPIO_PORT		GPIOD
#define UART_TX_PIN			GPIO_PIN_5
#define UART_RX_PIN			GPIO_PIN_6

struct USART_Handle_t Test_USART;

uint8_t check_sensor_response(void);
uint8_t read_dht11_sensor(uint8_t *data);

void configure_uart(void);
void UART_SendChar(uint8_t ch);

int main(void)
{
	uint8_t dht11_sensor_response;
	uint8_t dht11_sensor_data[5];

	//Configure the Timer
	configure_delay_timer();

	//Configure the UART Peripheral
	configure_uart();

	while(1)
	{
		dht11_sensor_response = read_dht11_sensor(dht11_sensor_data);

		if(dht11_sensor_response)
		{
			printf("Relative Humidity: %d.%d\r\n",dht11_sensor_data[0],dht11_sensor_data[1]);
			printf("Temperature: %d.%d\r\n",dht11_sensor_data[2],dht11_sensor_data[3]);
		}
		else
		{
			printf("Sensor Read Failed\r\n");
		}

		delay_us(60000000);
	}

	return 0;
}

uint8_t check_sensor_response(void)
{
	uint8_t sample[2];

	//Read First Sample
	delay_us(40);
	sample[0] = GPIOReadPin(DHT11_SENSOR_PORT, DHT11_SENSOR_DATA_PIN);

	//Read Second Sample
	delay_us(80);
	sample[1] = GPIOReadPin(DHT11_SENSOR_PORT, DHT11_SENSOR_DATA_PIN);

	//Wait till the end of the response
	while((GPIOReadPin(DHT11_SENSOR_PORT, DHT11_SENSOR_DATA_PIN)));

	if(sample[0]==GPIO_LOW && sample[1]==GPIO_HIGH)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t read_dht11_sensor(uint8_t *data)
{
	uint8_t sensor_response;
	uint8_t current_bit;
	uint8_t i;

	//GPIO Configuration for Sensor Data Pin
	EnablePeriClk(DHT11_SENSOR_PORT);
	GPIOSetMode(DHT11_SENSOR_PORT, DHT11_SENSOR_DATA_PIN, GPIO_MODE_OUTPUT);
	GPIOSetOutputType(DHT11_SENSOR_PORT,DHT11_SENSOR_DATA_PIN,GPIO_OPTYPE_OD);
	GPIOSetOutputSpeed(DHT11_SENSOR_PORT,DHT11_SENSOR_DATA_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(DHT11_SENSOR_PORT,DHT11_SENSOR_DATA_PIN,GPIO_PULL_UP);

	//Triggering the Sensor
	delay_us(18000);
	GPIOSetMode(DHT11_SENSOR_PORT, DHT11_SENSOR_DATA_PIN, GPIO_MODE_INPUT);

	//Checking for Sensor Response
	sensor_response = check_sensor_response();

	//Reading the Sensor Data
	if(sensor_response == 1)
	{
		for(i=0;i<40;i++)
		{
			while(!(GPIOReadPin(DHT11_SENSOR_PORT, DHT11_SENSOR_DATA_PIN)));
			delay_us(40);
			current_bit = GPIOReadPin(DHT11_SENSOR_PORT, DHT11_SENSOR_DATA_PIN);
			if(current_bit == GPIO_HIGH)
			{
				data[(i/8)] |= (1 << (7-(i%8)));
			}
			else
			{
				data[(i/8)] &= ~(1 << (7-(i%8)));
			}
			while((GPIOReadPin(DHT11_SENSOR_PORT, DHT11_SENSOR_DATA_PIN)));
		}
	}

	return sensor_response;
}

void configure_uart(void)
{
	//GPIO Pin Configuration
	EnablePeriClk(UART_GPIO_PORT);
	GPIOSetMode(UART_GPIO_PORT,UART_TX_PIN,GPIO_MODE_ALTFN);
	GPIOSetMode(UART_GPIO_PORT,UART_RX_PIN,GPIO_MODE_ALTFN);
	GPIOSetAltFn(UART_GPIO_PORT,UART_TX_PIN,GPIO_ALTFN_7);
	GPIOSetAltFn(UART_GPIO_PORT,UART_RX_PIN,GPIO_ALTFN_7);
	GPIOSetOutputType(UART_GPIO_PORT,UART_TX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputType(UART_GPIO_PORT,UART_RX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputSpeed(UART_GPIO_PORT,UART_TX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetOutputSpeed(UART_GPIO_PORT,UART_RX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(UART_GPIO_PORT,UART_TX_PIN,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(UART_GPIO_PORT,UART_RX_PIN,GPIO_PULL_UP);

	//USART Configuration
	Test_USART.pUSART = (struct USART_RegDef_t *) USART2;
	Test_USART.USART_Config.USART_Mode = USART_MODE_TX_RX;
	Test_USART.USART_Config.USART_DataLength = USART_DATA_LEN_8_BITS;
	Test_USART.USART_Config.USART_StopBits = USART_STOP_BITS_1;
	Test_USART.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	Test_USART.USART_Config.USART_BaudRate = USART_SB_RATE_9600;
	Test_USART.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CNTRL_NONE;

	EnablePeriClk(USART2);
	USART_Init(&Test_USART);
	USART_PeripheralEnable(&Test_USART);
}

void UART_SendChar(uint8_t ch)
{
	USART_SendData(&Test_USART, &ch, 1);
}
