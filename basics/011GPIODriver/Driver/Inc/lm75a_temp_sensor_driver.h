/*
 * lm75a_temp_sensor_driver.h
 * Driver for the LM75a I2C Temperature Sensor from NXP Semiconductors
 * Reference: https://www.nxp.com/docs/en/data-sheet/LM75A.pdf
 *  Created on: 25-Jan-2024
 *      Author: naveen
 */

#ifndef INC_LM75A_TEMP_SENSOR_DRIVER_H_
#define INC_LM75A_TEMP_SENSOR_DRIVER_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"

uint16_t lm75a_read_temperature(void);
void lm75a_display_temperature_lcd(uint16_t temp);

#endif /* INC_LM75A_TEMP_SENSOR_DRIVER_H_ */
