/*
 * lm75a_temp_sensor_driver.c
 * Driver for the LM75a I2C Temperature Sensor from NXP Semiconductors
 * Reference: https://www.nxp.com/docs/en/data-sheet/LM75A.pdf
 *  Created on: 25-Jan-2024
 *      Author: naveen
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "lm75a_temp_sensor_driver.h"
#include "common_utils.h"
#include "external_interrupt_config.h"

extern uint32_t *I2C_DEVICE_LM75A;
extern uint8_t SlaveAddressLM75A;

uint16_t lm75a_read_temperature(void)
{

	return 0;
}

void lm75a_display_temperature_lcd(uint16_t temp)
{
	uint16_t lm75a_temp[2];
	uint8_t temp_fractional_part;

	//Integer part of the temperature
	lm75a_temp[0] = (uint8_t) (temp >> 8);

	//Fractional part of the temperature
	temp_fractional_part = (uint8_t) temp;
	temp_fractional_part = temp_fractional_part >> 5;

	lm75a_temp[1] = 125*temp_fractional_part;

	lcd_pcf8574_clear_screen();
	delay_us(3000);
	lcd_pcf8574_return_home();
	delay_us(3000);
	printf("TEMP: %02d.%03d C",lm75a_temp[0],lm75a_temp[1]);

	return;
}