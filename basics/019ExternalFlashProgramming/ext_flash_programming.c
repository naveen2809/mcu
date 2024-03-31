#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "usart_driver.h"
#include "common_utils.h"
#include "at45dbxx_flash_memory_driver.h"

#define CMD_BUFFER_LENGTH		5
#define CMD_EXT_FLASH_WRITE		0x1
#define CMD_EXT_FLASH_READ		0x2

#define	DATA_BUFFER_LENGTH		256

char CmdBuffer[CMD_BUFFER_LENGTH];

char DataBuffer[DATA_BUFFER_LENGTH];
uint32_t DataLen;

uint8_t tx_buffer[DATA_BUFFER_LENGTH];
uint8_t rx_buffer[DATA_BUFFER_LENGTH];

extern struct USART_Handle_t Test_USART;

int main(void)
{
	uint8_t result_buf_write, result_page_proram;
	uint32_t i;

	//Configuring the UART
	configure_uart();

	//Configure the SPI Peripheral
	configure_spi();

	
	//Erasing the Chip
	//result = at45dbxx_chip_erase();
	//delay_us(100000);
	
	//Configure the FLASH Module
	at45dbxx_configure_flash();

	setbuf(stdout, NULL);

	
	/*
	if(result == AT45DBXX_SUCCESS)
	{
		printf("The external flash chip is erased successfully\r\n");
	}
	else
	{
		printf("The external flash chip erase failed\r\n");
	}
	*/

	//Read and Print the Manufacturer and Device Information
	at45dbxx_read_manufacturer_and_device_info(rx_buffer);

	printf("Manufacturer and Device ID: ");
	for(i=0;i<AT45DBXX_MANUFACTURER_AND_DEVICE_INFO_LEN;i++)
	{
		printf("%X ",rx_buffer[i]);
	}
	printf("\r\n");


	while(1)
	{
		USART_ReceiveData(&Test_USART,(uint8_t *)CmdBuffer,CMD_BUFFER_LENGTH);
		printf("CMD: %x, EXT FLASH ADDRESS: %x %x %x, LENGTH: %x\r\n",CmdBuffer[0],CmdBuffer[1],CmdBuffer[2],CmdBuffer[3],CmdBuffer[4]);

		DataLen = (uint8_t) CmdBuffer[4];
		USART_ReceiveData(&Test_USART,(uint8_t *)DataBuffer,DataLen+1);
		

		//Writing the data to external flash
		result_buf_write = at45dbxx_buffer_write(AT45DBXX_SRAM_BUFFER_1,(uint8_t *)&CmdBuffer[1],(uint8_t *)DataBuffer,DataLen+1);
		result_page_proram = at45dbxx_buffer_to_page_program_with_erase(AT45DBXX_SRAM_BUFFER_1, (uint8_t *)&CmdBuffer[1]);

		if(result_buf_write == AT45DBXX_SUCCESS && result_page_proram == AT45DBXX_SUCCESS)
		{
			printf("Data length:%ld Data: %x %x Program Success\r\n",DataLen+1,DataBuffer[0],DataBuffer[1]);
		}
		else
		{
			printf("Data length:%ld Data: %x %x Program Failed\r\n",DataLen+1,DataBuffer[0],DataBuffer[1]);
		}


	}

	return 0;
}