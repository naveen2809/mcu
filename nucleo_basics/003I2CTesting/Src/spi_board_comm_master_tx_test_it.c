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
#define SLEEP_COUNT				5000000
#define ENABLE					1
#define SPI2_IRQ_NUM			36

struct SPI_Handle_t SPI2_Handle;

char TxBuf[] = "Message over SPI Interface from STM32F446RE Discovery Board using Interrupts!\r\n";
char TxBuf2[] = "Another message over SPI Interface from the same Board using Interrupts!\r\n";
uint8_t MessageLen[2];

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

void SPI2_IRQHandler(void)
{
	SPI_IRQ_Handler(&SPI2_Handle);
}

int main(void)
{
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
	SPI2_Handle.pSPI = (struct SPI_RegDef_t *) SPI2;
	SPI2_Handle.SPI_Config.SPIDeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2_Handle.SPI_Config.SPIClockPol = SPI_CLK_POL_0;
	SPI2_Handle.SPI_Config.SPIClockPhase = SPI_CLK_PHA_LE;
	SPI2_Handle.SPI_Config.SPIClockFreq = SPI_CLK_FREQ_DIV256;
	SPI2_Handle.SPI_Config.SPIDataFrameFormat = SPI_DFF_8_BITS;
	SPI2_Handle.SPI_Config.SPISoftwareSlaveManagement = SPI_SW_SLAVE_MGNT_DI;
	SPI2_Handle.SPI_Config.SPISSIFlag = SPI_SSI_1;
	SPI2_Handle.SPI_Config.SPISSOEFlag = SPI_SSOE_EN;
	SPI2_Handle.SPI_Peripheral_Status=SPI_PERIPHERAL_READY;
	SPI2_Handle.TxBuf=NULL;
	SPI2_Handle.Txlen=0;
	SPI2_Handle.RxBuf=NULL;
	SPI2_Handle.Rxlen=0;

	EnablePeriClk((uint32_t*)SPI2_Handle.pSPI);
	SPIPeriConfig((uint32_t*)SPI2_Handle.pSPI, &SPI2_Handle.SPI_Config);
	SPIEnable((uint32_t*)SPI2_Handle.pSPI);

	//Enabling the IRQ for SPI2 Peripheral
	NVIC_EnableIRQ(SPI2_IRQ_NUM);


	while(1)
	{
		while(GPIOReadPin(GPIOC,GPIO_PIN_NUM_INPUT) == GPIO_HIGH);
		GPIOTogglePin(GPIOA,GPIO_PIN_NUM_OUTPUT);
		delay();
		MessageLen[0] = (uint8_t) strlen(TxBuf);
		MessageLen[1] = (uint8_t) strlen(TxBuf);
		SPIMasterSendDataIT(&SPI2_Handle,MessageLen, 2);


		//while(GPIOReadPin(GPIOC,GPIO_PIN_NUM_INPUT) == GPIO_HIGH);
		GPIOTogglePin(GPIOA,GPIO_PIN_NUM_OUTPUT);
		delay();
		SPIMasterSendDataIT(&SPI2_Handle, (uint8_t *) TxBuf, (uint32_t) strlen(TxBuf));


		//while(GPIOReadPin(GPIOC,GPIO_PIN_NUM_INPUT) == GPIO_HIGH);
		GPIOTogglePin(GPIOA,GPIO_PIN_NUM_OUTPUT);
		delay();
		MessageLen[0] = (uint8_t) strlen(TxBuf2);
		MessageLen[1] = (uint8_t) strlen(TxBuf2);
		SPIMasterSendDataIT(&SPI2_Handle,MessageLen, 2);

		//while(GPIOReadPin(GPIOC,GPIO_PIN_NUM_INPUT) == GPIO_HIGH);
		GPIOTogglePin(GPIOA,GPIO_PIN_NUM_OUTPUT);
		delay();
		SPIMasterSendDataIT(&SPI2_Handle, (uint8_t *) TxBuf2, (uint32_t) strlen(TxBuf2));
	}

	SPIDisable((uint32_t*)SPI2_Handle.pSPI);
	DisablePeriClk((uint32_t*)SPI2_Handle.pSPI);

	return 0;
}
