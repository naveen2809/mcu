/*
 * usart_test.c
 *
 *  Created on: 30-Jul-2023
 *      Author: naveen
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "spi_driver.h"
#include "gpio_driver.h"

#define SPI_PIN_MOSI			GPIO_PIN_15
#define SPI_PIN_MISO			GPIO_PIN_14
#define SPI_PIN_SCLK			GPIO_PIN_13
#define SPI_PIN_SS				GPIO_PIN_12
#define GPIO_PIN_NUM_INPUT		GPIO_PIN_13
#define GPIO_PIN_NUM_OUTPUT		GPIO_PIN_5
#define SLEEP_COUNT				500000
#define ENABLE					1

char TxBuf[] = "Message over SPI Interface from STM32F446RE Discovery Board!\r\n";
char TxBuf2[] = "Another message over SPI Interface from the same Board!\r\n";
uint8_t MessageLen[2];

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{
	struct SPI_Config_t SPI_Dev_Config;

	//GPIO Pin Configuration for Button and LED
	EnablePeriClk(GPIOA);
	EnablePeriClk(GPIOC);
	GPIOSetMode(GPIOA,GPIO_PIN_NUM_OUTPUT,GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOC,GPIO_PIN_NUM_INPUT,GPIO_MODE_INPUT);

	//GPIO Pin Configuration for SPI Port
	EnablePeriClk(GPIOB);
	GPIOSetMode(GPIOB,SPI_PIN_MOSI,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOB,SPI_PIN_MISO,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOB,SPI_PIN_SCLK,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOB,SPI_PIN_SS,GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOB,SPI_PIN_MOSI,GPIO_ALTFN_5);
	GPIOSetAltFn(GPIOB,SPI_PIN_MISO,GPIO_ALTFN_5);
	GPIOSetAltFn(GPIOB,SPI_PIN_SCLK,GPIO_ALTFN_5);
	GPIOSetAltFn(GPIOB,SPI_PIN_SS,GPIO_ALTFN_5);

	//SPI Port Configuration
	SPI_Dev_Config.SPIDeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI_Dev_Config.SPIClockPol = SPI_CLK_POL_0;
	SPI_Dev_Config.SPIClockPhase = SPI_CLK_PHA_LE;
	SPI_Dev_Config.SPIClockFreq = SPI_CLK_FREQ_DIV256;
	SPI_Dev_Config.SPIDataFrameFormat = SPI_DFF_8_BITS;
	SPI_Dev_Config.SPISoftwareSlaveManagement = SPI_SW_SLAVE_MGNT_DI;
	SPI_Dev_Config.SPISSIFlag = SPI_SSI_1;
	SPI_Dev_Config.SPISSOEFlag = SPI_SSOE_EN;

	EnablePeriClk(SPI2);
	SPIPeriConfig(SPI2, &SPI_Dev_Config);
	SPIEnable(SPI2);

	while(1)
	{
		/* UART Tx Using Polling*/

		while(GPIOReadPin(GPIOC,GPIO_PIN_NUM_INPUT) == GPIO_HIGH);
		GPIOTogglePin(GPIOA,GPIO_PIN_NUM_OUTPUT);
		MessageLen[0] = (uint8_t) strlen(TxBuf);
		MessageLen[1] = (uint8_t) strlen(TxBuf);
		SPISendData(SPI2, MessageLen, 2);
		SPISendData(SPI2, (uint8_t *) TxBuf, (uint32_t) strlen(TxBuf));
		delay();

		while(GPIOReadPin(GPIOC,GPIO_PIN_NUM_INPUT) == GPIO_HIGH);
		GPIOTogglePin(GPIOA,GPIO_PIN_NUM_OUTPUT);
		MessageLen[0] = (uint8_t) strlen(TxBuf2);
		MessageLen[1] = (uint8_t) strlen(TxBuf2);
		SPISendData(SPI2, MessageLen, 2);
		SPISendData(SPI2, (uint8_t *) TxBuf2, (uint32_t) strlen(TxBuf2));
		delay();
	}

	SPIDisable(SPI2);
	DisablePeriClk(SPI2);

	return 0;
}
