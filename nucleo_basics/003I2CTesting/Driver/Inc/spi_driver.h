/*
 * spi_driver.h
 *
 *  Created on: 28-Jun-2023
 *      Author: naveen
 */

#ifndef INC_SPI_DRIVER_H_
#define INC_SPI_DRIVER_H_

#include <stdint.h>
#include "stm32f4xx.h"

struct SPI_RegDef_t
{
	uint32_t SPI_CR1;
	uint32_t SPI_CR2;
	uint32_t SPI_SR;
	uint32_t SPI_DR;
	uint32_t SPI_CRCPR;
	uint32_t SPI_RXCRCR;
	uint32_t SPI_TXCRCR;
	uint32_t SPI_I2SCFGR;
	uint32_t SPI_I2SPR;
};

struct SPI_Config_t
{
	uint8_t SPIClockPol;
	uint8_t SPIClockPhase;
	uint8_t SPIDeviceMode;
	uint8_t SPIDuplexMode;
	uint8_t SPIClockFreq;
	uint8_t SPIDataFrameFormat;
	uint8_t SPISoftwareSlaveManagement;
	uint8_t SPISSIFlag;
	uint8_t SPISSOEFlag;
};

struct SPI_Handle_t
{
	struct SPI_RegDef_t *pSPI;
	struct SPI_Config_t SPI_Config;
	uint8_t SPI_Peripheral_Status;
	uint8_t *TxBuf;
	uint8_t Txlen;
	uint8_t *RxBuf;
	uint8_t Rxlen;
};

#define SPI_CR1_CPHA			0
#define SPI_CR1_CPOL			1
#define SPI_CR1_MSTR			2
#define SPI_CR1_BR				3
#define SPI_CR1_SPE				6
#define SPI_CR1_LSBFIRST		7
#define SPI_CR1_SSI				8
#define SPI_CR1_SSM				9
#define SPI_CR1_RXONLY			10
#define SPI_CR1_DFF				11
#define SPI_CR1_CRCNEXT			12
#define SPI_CR1_CRCEN			13
#define SPI_CR1_BIDIOE			14
#define SPI_CR1_BIDIMODE		15

#define SPI_CR2_RXDMAEN			0
#define SPI_CR2_TXDMAEN			1
#define SPI_CR2_SSOE			2
#define SPI_CR2_FRF				4
#define SPI_CR2_ERRIE			5
#define SPI_CR2_RXNEIE			6
#define SPI_CR2_TXEIE			7

#define SPI_SR_RXNE				0
#define SPI_SR_TXE				1
#define SPI_SR_CHSIDE			2
#define SPI_SR_UDR				3
#define SPI_SR_CRCERR			4
#define SPI_SR_MODF				5
#define SPI_SR_OVR				6
#define SPI_SR_BSY				7
#define SPI_SR_FRE				8

#define SPI_DEVICE_MODE_MASTER	1
#define SPI_DEVICE_MODE_SLAVE	0
#define SPI_CLK_POL_0			0
#define SPI_CLK_POL_1			1
#define SPI_CLK_PHA_LE			0
#define SPI_CLK_PHA_TE			1
#define SPI_CLK_FREQ_DIV2		0x0
#define SPI_CLK_FREQ_DIV4		0x1
#define SPI_CLK_FREQ_DIV8		0x2
#define SPI_CLK_FREQ_DIV16		0x3
#define SPI_CLK_FREQ_DIV32		0x4
#define SPI_CLK_FREQ_DIV64		0x5
#define SPI_CLK_FREQ_DIV128		0x6
#define SPI_CLK_FREQ_DIV256		0x7
#define SPI_DFF_8_BITS			0
#define SPI_DFF_16_BITS			1
#define SPI_SW_SLAVE_MGNT_DI	0
#define SPI_SW_SLAVE_MGNT_EN	1
#define SPI_SSI_0			0
#define SPI_SSI_1			1
#define SPI_SSOE_EN		1
#define SPI_SSOE_DI		0
#define SPI_PERIPHERAL_READY		0
#define SPI_PERIPHERAL_BUSY			1

#define APP_EVENT_TX_CMPLT	0
#define APP_EVENT_RX_CMPLT	1

void SPIPeriConfig(uint32_t *SPIAddress, struct SPI_Config_t *pSPIConfig);
void SPIEnable(uint32_t *SPIAddress);
void SPIDisable(uint32_t *SPIAddress);
uint8_t SPIGetFlags(uint32_t *SPIAddress, uint8_t Flag);
void SPISendData(uint32_t *SPIAddress, uint8_t *TxBuf, uint8_t len);
void SPIRecvData(uint32_t *SPIAddress, uint8_t *TxBuf, uint8_t len);
void SPIMasterSendDataIT(struct SPI_Handle_t *pSPI_Handle, uint8_t *TxBuf, uint8_t len);
void SPIMasterRecvDataIT(struct SPI_Handle_t *pSPI_Handle, uint8_t *TxBuf, uint8_t *RxBuf, uint8_t len);
void SPISlaveSendDataIT(struct SPI_Handle_t *pSPI_Handle, uint8_t *TxBuf, uint8_t *RxBuf, uint8_t len);
void SPISlaveRecvDataIT(struct SPI_Handle_t *pSPI_Handle, uint8_t *RxBuf, uint8_t len);
void SPI_IRQ_Handler(struct SPI_Handle_t *pSPI_Handle);
extern void SPI_ApplicationCallback(uint8_t Event);

#endif /* INC_SPI_DRIVER_H_ */
