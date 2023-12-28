/*
 * dht11_driver.h
 *
 *  Created on: 24-Dec-2023
 *      Author: naveen
 */

#ifndef INC_DHT11_DRIVER_H_
#define INC_DHT11_DRIVER_H_

#define DHT11_SENSOR_DATA_PIN			GPIO_PIN_0
#define DHT11_SENSOR_PORT				GPIOC

uint8_t check_sensor_response(void);
uint8_t read_dht11_sensor(uint8_t *data);

#endif /* INC_DHT11_DRIVER_H_ */
