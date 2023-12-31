/*
 * temp_on_lcd.c
 * Test code for the AT24CXX I2C EEPROM
 * Reference: https://ww1.microchip.com/downloads/en/devicedoc/doc0670.pdf
 *  Created on: 25-Dec-2023
 *      Author: naveen
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "i2c_driver.h"
#include "general_purpose_timer.h"
#include "at24cxx_eeprom_driver.h"
#include "common_utils.h"

#define READ_BUFFER_SIZE 128

int main(void)
{
	uint16_t eeprom_address = 0x4100;		//First byte of Page 1 (Address 65). Page size of EEPROM is 64 bytes
	char message[]="Hello, World!!!";
	char read_buffer[READ_BUFFER_SIZE];

	uint16_t length,i;

	length = strlen(message);
	length = 10;

	//Configure the Timer
	configure_delay_timer();

	//Configure the UART
	configure_uart();

	//Configure the I2C Peripheral
	configure_i2c();
/*
	//Write Byte to EEPROM
	eeprom_at24cxx_write_byte(eeprom_address,(uint8_t *)message);

	//Wait for EEPROM Write to be Completed
	delay_us(30000);

	//Read Byte from EEPROM
	eeprom_at24cxx_read_byte(eeprom_address,(uint8_t *)read_buffer);
	printf("Read Byte from Address %x: %c\r\n",eeprom_address,read_buffer[0]);   //Ensure that printf() is redirected to UART in "syscalls.c" file

	//Write String of Bytes to EEPROM
	length = strlen(message);
	eeprom_at24cxx_write_page(eeprom_address,(uint8_t *)message,length);

	//Wait for EEPROM Write to be Completed
	delay_us(5000000);
*/
	//Read String of Bytes from EEPROM
	eeprom_at24cxx_read_sequential(eeprom_address,(uint8_t *)read_buffer,length);

	//Print the bytes read from EEPROM
	printf("Read String of Bytes from Address %x: ",eeprom_address);   //Ensure that printf() is redirected to UART in "syscalls.c" file
	for(i=0;i<length;i++)
		printf("%c",read_buffer[i]);
	printf("\r\n");

	while(1)
	{

	}

	return 0;
}
