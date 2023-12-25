/*
 * at24cxx_eeprom_driver.c
 * Driver for Atmel/Microchip AT24CXX I2C EEPROM Devices
 * Reference: https://ww1.microchip.com/downloads/en/devicedoc/doc0670.pdf
 *  Created on: 25-Dec-2023
 *      Author: naveen
 */

#include "at24cxx_eeprom_driver.h"
#include "common_utils.h"

extern uint32_t * I2C_DEVICE;
extern uint8_t SlaveAddress;

void eeprom_at24cxx_read_byte(uint16_t eeprom_address,uint8_t *buffer)
{
	I2CMaster_ATC24xx_EEPROM_Read_Sequential(I2C_DEVICE,buffer,1,eeprom_address,SlaveAddress);
	return;
}

void eeprom_at24cxx_read_byte_current_address(uint8_t *buffer)
{
	I2CMasterReceiveData(I2C_DEVICE,buffer,1,SlaveAddress);
	return;
}

void eeprom_at24cxx_read_sequential(uint16_t eeprom_address,uint8_t *buffer,uint16_t length)
{
	I2CMaster_ATC24xx_EEPROM_Read_Sequential(I2C_DEVICE,buffer,length,eeprom_address,SlaveAddress);
	return;
}

void eeprom_at24cxx_read_sequential_current_address(uint8_t *buffer,uint16_t length)
{
	I2CMasterReceiveData(I2C_DEVICE,buffer,length,SlaveAddress);
	return;
}

void eeprom_at24cxx_write_byte(uint16_t eeprom_address,uint8_t *buffer)
{
	uint8_t * data;

	data = (uint8_t *)malloc(3);

	if(data !=NULL)
	{
		data[0] = (uint8_t) eeprom_address;
		data[1] = (uint8_t) (eeprom_address >> 8);
		data[2] = buffer[0];

		I2CMasterSendData(I2C_DEVICE,data,3,SlaveAddress);
	}

	return;
}

void eeprom_at24cxx_write_page(uint16_t eeprom_address,uint8_t *buffer,uint16_t length)
{
	uint8_t * data;
	uint8_t i;

	data = (uint8_t *)malloc(length+2);

	if(data !=NULL)
	{
		data[0] = (uint8_t) eeprom_address;
		data[1] = (uint8_t) (eeprom_address >> 8);

		for(i=0;i<length;i++)
		{
			data[i+2] = buffer[i];
		}

		I2CMasterSendData(I2C_DEVICE,data,length+2,SlaveAddress);
	}

	return;
}
