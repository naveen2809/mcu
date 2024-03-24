/*

 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "usart_driver.h"
#include "general_purpose_timer.h"
#include "common_utils.h"

#define LEN 100

char CmdBuffer[LEN];
uint32_t CmdLen;

extern struct USART_Handle_t Test_USART;

void handle_command(void);
void handle_command_get_version(void);
void handle_command_sector_erase(void);
void handle_command_mass_erase(void);
void handle_command_start_app(void);

uint32_t getcommandname(char *src,char *dst,uint32_t len);
uint32_t getsector(char *src,uint32_t len);
uint8_t mystrcmp(char *src,char *dst,uint32_t len);
uint32_t get_power(uint32_t exponent);

int main(void)
{
	//Configuring the timer
	configure_delay_timer();

	//Configuring the UART
	configure_uart();

	setbuf(stdout, NULL);

	printf("\r\n");

	while(1)
	{
		printf("$ ");
		USART_ReceiveCmd(&Test_USART,(uint8_t *)CmdBuffer,&CmdLen);
		if(CmdLen > 1)
		{
			handle_command();
		}
	}

	return 0;
}

void handle_command(void)
{
	char command_name[LEN];
	uint32_t command_name_length;

	CmdBuffer[CmdLen-1] = '\0';

	command_name_length = getcommandname(CmdBuffer,command_name,CmdLen);

	if(mystrcmp(command_name,"version",command_name_length))
	{
		handle_command_get_version();
	}
	else if(mystrcmp(command_name,"sector_erase",command_name_length))
	{
		handle_command_sector_erase();
	}
	else if(mystrcmp(command_name,"mass_erase",command_name_length))
	{
		handle_command_mass_erase();
	}
	else if(mystrcmp(command_name,"start_app",command_name_length))
	{
		handle_command_start_app();
	}

	return;
}

void handle_command_get_version(void)
{
	printf("Bootloader v0.1\r\n");

	return;
}

void handle_command_sector_erase(void)
{
	uint32_t sector;

	sector = getsector(CmdBuffer,CmdLen);
	if(sector >= 0 && sector<=7)
	{
		printf("Sector %ld erased successfully\r\n",sector);
	}
	else
	{
		printf("Invalid sector\r\n");
	}

	return;
}

void handle_command_mass_erase(void)
{
	printf("Flash erased successfully\r\n");

	return;
}

void handle_command_start_app(void)
{
	printf("Starting application\r\n");

	return;
}

uint32_t getcommandname(char *src,char *dst,uint32_t len)
{
	uint32_t i, cmd_length;
	cmd_length = 0;

	for(i=0;i<len;i++)
	{
		if(src[i] == ' ')
		{
			dst[i] = '\0';
			cmd_length++;
			break;
		}
		else
		{
			dst[i] = src[i];
			cmd_length++;
		}
	}

	return cmd_length;
}

uint32_t getsector(char *src,uint32_t len)
{
	uint32_t i, sector_length, sector,exponent,power;
	char sector_string[100];

	i = 0;
	sector_length = 0;
	sector = 0;

	while(src[i] != ' ')
	{
		i++;
	}

	i++;

	while(i<len)
	{
		if(src[i] == ' ')
		{
			sector_string[sector_length] = '\0';
			sector_length++;
			i++;
			break;
		}
		else
		{
			sector_string[sector_length] = src[i];
			sector_length++;
			i++;
		}
	}

	exponent = sector_length - 2;
	for(i=0;i<sector_length-1;i++)
	{
		power = get_power(exponent);
		sector += (sector_string[i]-48)*power;
	}

	return sector;
}

uint8_t mystrcmp(char *src,char *dst,uint32_t len)
{
	uint32_t i,count;

	count = 0;
	for(i=0;i<len;i++)
	{
		if(src[i] == dst[i])
		{
			count++;
		}
		else
		{
			break;
		}
	}

	if(count == len)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint32_t get_power(uint32_t exponent)
{
	uint32_t i, num = 1;

	for(i=0;i<exponent;i++)
	{
		num = num * 10;
	}

	return num;
}
