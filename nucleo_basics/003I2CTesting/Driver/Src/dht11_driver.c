/*
 * dht11_driver.c
 *
 *  Created on: 24-Dec-2023
 *      Author: naveen
 */

#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "common_utils.h"
#include "dht11_driver.h"

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
