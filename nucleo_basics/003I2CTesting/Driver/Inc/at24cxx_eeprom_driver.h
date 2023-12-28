/*
 * at24cxx_eeprom_driver.h
 * Driver for Atmel/Microchip AT24CXX I2C EEPROM Devices
 * Reference: https://ww1.microchip.com/downloads/en/devicedoc/doc0670.pdf
 *  Created on: 25-Dec-2023
 *      Author: naveen
 */

#ifndef INC_AT24CXX_EEPROM_DRIVER_H_
#define INC_AT24CXX_EEPROM_DRIVER_H_

#include <stdint.h>

void eeprom_at24cxx_read_byte(uint16_t eeprom_address,uint8_t *buffer);
void eeprom_at24cxx_read_byte_current_address(uint8_t *buffer);
void eeprom_at24cxx_read_sequential(uint16_t eeprom_address,uint8_t *buffer,uint16_t length);
void eeprom_at24cxx_read_sequential_current_address(uint8_t *buffer,uint16_t length);
void eeprom_at24cxx_write_byte(uint16_t eeprom_address,uint8_t *buffer);
void eeprom_at24cxx_write_page(uint16_t eeprom_address,uint8_t *buffer,uint16_t length);

#endif /* INC_AT24CXX_EEPROM_DRIVER_H_ */
