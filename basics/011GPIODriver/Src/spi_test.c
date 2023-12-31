/*
 * spi_test.c
 *
 *  Created on: 28-Jun-2023
 *      Author: naveen
 */


#include <stdint.h>
#include "spi_driver.h"
#include "gpio_driver.h"
#include <string.h>

#define MAX_DATA 500
#define SLEEP_COUNT 10000

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{
	char send_data[] = "A small world after all"
			"";
	char recv_data[MAX_DATA];
	uint8_t data_len = 11;

	struct SPI_Config_t SPI_Dev_Config;

	SPI_Dev_Config.SPIDeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI_Dev_Config.SPIClockPol = SPI_CLK_POL_0;
	SPI_Dev_Config.SPIClockPhase = SPI_CLK_PHA_LE;
	SPI_Dev_Config.SPIClockFreq = SPI_CLK_FREQ_DIV4;
	SPI_Dev_Config.SPIDataFrameFormat = SPI_DFF_8_BITS;
	SPI_Dev_Config.SPISoftwareSlaveManagement = SPI_SW_SLAVE_MGNT_DI;
	SPI_Dev_Config.SPISSIFlag = SPI_SSI_1;
	SPI_Dev_Config.SPISSOEFlag = SPI_SSOE_EN;

	EnablePeriClk(GPIOB);
	GPIOSetMode(GPIOB,GPIO_PIN_12,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOB,GPIO_PIN_13,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOB,GPIO_PIN_14,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOB,GPIO_PIN_15,GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOB,GPIO_PIN_12,GPIO_ALTFN_5);
	GPIOSetAltFn(GPIOB,GPIO_PIN_13,GPIO_ALTFN_5);
	GPIOSetAltFn(GPIOB,GPIO_PIN_14,GPIO_ALTFN_5);
	GPIOSetAltFn(GPIOB,GPIO_PIN_15,GPIO_ALTFN_5);

	EnablePeriClk(SPI2);
	SPIPeriConfig(SPI2, &SPI_Dev_Config);
	SPIEnable(SPI2);

	SPISendData(SPI2,(uint8_t*)data_len,1);
	delay();
	SPISendData(SPI2,(uint8_t*)send_data,strlen(send_data));

	//SPIRecvData(SPI2,(uint8_t*)recv_data,strlen(recv_data));

	SPIDisable(SPI2);

	while(1);

	return 0;

}
