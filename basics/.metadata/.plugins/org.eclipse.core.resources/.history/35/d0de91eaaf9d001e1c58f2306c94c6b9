/*
 * i2c_tx_test.c
 *
 *  Created on: 02-Aug-2023
 *      Author: naveen
 */



#include "stm32f4xx.h"
#include "spi_driver.h"
#include "usart_driver.h"
#include "gpio_driver.h"
#include <stdint.h>
#include <string.h>

#define UART_TX_PIN						GPIO_PIN_2
#define UART_RX_PIN						GPIO_PIN_3
#define SPI_PIN_MOSI					GPIO_PIN_15
#define SPI_PIN_MISO					GPIO_PIN_14
#define SPI_PIN_SCLK					GPIO_PIN_13
#define SPI_PIN_SS						GPIO_PIN_12
#define GPIO_PIN_NUM_OUTPUT_1			GPIO_PIN_14
#define GPIO_PIN_NUM_OUTPUT_2			GPIO_PIN_15
#define GPIO_PIN_NUM_OUTPUT_3			GPIO_PIN_13
#define SLEEP_COUNT						500000

struct USART_Handle_t Test_USART;

char message[128];
uint8_t MessageLen[2];

static void NumToString(uint8_t Num, char *NumString);

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{

	struct SPI_Config_t SPI_Dev_Config;

	char MessageLenString[6] = {48,49,50,'\r','\n'};
	char MessageLenStringPrefix[] = "Message Len: ";

	//GPIO Pin Configuration for LEDs
	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOD,GPIO_PIN_NUM_OUTPUT_1,GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD,GPIO_PIN_NUM_OUTPUT_2,GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD,GPIO_PIN_NUM_OUTPUT_3,GPIO_MODE_OUTPUT);

	//GPIO Pin Configuration for SPI
	EnablePeriClk(GPIOB);
	GPIOSetMode(GPIOB,SPI_PIN_MOSI,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOB,SPI_PIN_MISO,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOB,SPI_PIN_SCLK,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOB,SPI_PIN_SS,GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOB,SPI_PIN_MOSI,GPIO_ALTFN_5);
	GPIOSetAltFn(GPIOB,SPI_PIN_MISO,GPIO_ALTFN_5);
	GPIOSetAltFn(GPIOB,SPI_PIN_SCLK,GPIO_ALTFN_5);
	GPIOSetAltFn(GPIOB,SPI_PIN_SS,GPIO_ALTFN_5);

	//GPIO Pin Configuration for USART2
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA,UART_TX_PIN,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOA,UART_RX_PIN,GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,UART_TX_PIN,GPIO_ALTFN_7);
	GPIOSetAltFn(GPIOA,UART_RX_PIN,GPIO_ALTFN_7);
	GPIOSetOutputType(GPIOA,UART_TX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputType(GPIOA,UART_RX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputSpeed(GPIOA,UART_TX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetOutputSpeed(GPIOA,UART_RX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(GPIOA,UART_TX_PIN,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(GPIOA,UART_RX_PIN,GPIO_PULL_UP);

	//SPI Port Configuration
	SPI_Dev_Config.SPIDeviceMode = SPI_DEVICE_MODE_SLAVE;
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

	//USART2 Port Configuration
	Test_USART.pUSART = (struct USART_RegDef_t *) USART2;
	Test_USART.USART_Config.USART_Mode = USART_MODE_TX_RX;
	Test_USART.USART_Config.USART_DataLength = USART_DATA_LEN_8_BITS;
	Test_USART.USART_Config.USART_StopBits = USART_STOP_BITS_1;
	Test_USART.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	Test_USART.USART_Config.USART_BaudRate = USART_SB_RATE_9600;
	Test_USART.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CNTRL_NONE;

	EnablePeriClk(USART2);
	USART_Init(&Test_USART);
	USART_PeripheralEnable(&Test_USART);

	while(1)
	{
		GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_1);
		SPIRecvData(SPI2,MessageLen,2);
		GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_2);
		SPIRecvData(SPI2,(uint8_t *)message,MessageLen[0]);
		GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_3);
		NumToString(MessageLen[0], MessageLenString);
		USART_SendData(&Test_USART, (uint8_t *)MessageLenStringPrefix, (uint32_t) strlen(MessageLenStringPrefix));
		USART_SendData(&Test_USART, (uint8_t *)MessageLenString, 5);
		USART_SendData(&Test_USART, (uint8_t *)message, (uint32_t) MessageLen[0]);
		delay();
	}

	//SPI Peripheral Disable
	SPIDisable(SPI2);
	DisablePeriClk(SPI2);

	//USART2 Peripheral Disable
	USART_PeripheralDisable(&Test_USART);
	DisablePeriClk(USART2);

	return 0;
}

static void NumToString(uint8_t Num, char *NumString)
{
	char temp;

	if(Num > 99)
	{
		temp = Num/100;
		Num = Num - temp * 100;
	}
	else
	{
		temp = 0;
	}

	*NumString = temp + 48;
	NumString++;

	if(Num > 9)
	{
			temp = Num/10;
			Num = Num - temp * 10;

	}
	else
	{
		temp = 0;
	}

	*NumString = temp + 48;
	NumString++;

	*NumString = Num + 48;
	NumString++;

	*NumString = '\r';
	NumString++;

	*NumString = '\n';

}
