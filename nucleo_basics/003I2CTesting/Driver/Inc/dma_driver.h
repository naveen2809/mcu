/*
 * dma_driver.h
 *
 *  Created on: Oct 26, 2023
 *      Author: naveen
 */

#ifndef INC_DMA_DRIVER_H_
#define INC_DMA_DRIVER_H_

#include "stm32f4xx.h"
#include <stdint.h>

struct DMA_Stream_RegDef_t
{
	uint32_t DMA_CR;
	uint32_t DMA_NDTR;
	uint32_t DMA_PAR;
	uint32_t DMA_M0AR;
	uint32_t DMA_M1AR;
	uint32_t DMA_FCR;
};

struct DMA_Controller_RegDef_t
{
	uint32_t DMA_LISR;
	uint32_t DMA_HISR;
	uint32_t DMA_LIFCR;
	uint32_t DMA_HIFCR;
};

struct DMA_Config_t
{
	uint32_t DMA_Channel;
	uint32_t DMA_Peripheral_Address;
	uint32_t DMA_M0_Address;
	uint32_t DMA_M1_Address;
	uint32_t DMA_Memory_Data_Size;
	uint32_t DMA_Peripheral_Data_Size;
	uint32_t DMA_Memory_Address_Increment;
	uint32_t DMA_Peripheral_Address_Increment;
	uint32_t DMA_Data_Transfer_Direction;
	uint32_t DMA_Stream_Priority_Level;
	uint32_t DMA_Num_Bytes;
	uint32_t DMA_TCIE_Status;
	uint32_t DMA_HTIE_Status;
};

struct DMA_Handle_t
{
	struct DMA_Controller_RegDef_t *pDMA_Cont;
	struct DMA_Stream_RegDef_t *pDMA_Stream;
	struct DMA_Config_t DMA_Config;
};

#define DMA_CR_EN			0
#define DMA_CR_DMEIE		1
#define DMA_CR_TEIE			2
#define DMA_CR_HTIE			3
#define DMA_CR_TCIE			4
#define DMA_CR_PFCTRL		5
#define DMA_CR_DIR			6
#define DMA_CR_CIRC			8
#define DMA_CR_PINC			9
#define DMA_CR_MINC			10
#define DMA_CR_PSIZE		11
#define DMA_CR_MSIZE		13
#define DMA_CR_PINCOS		15
#define DMA_CR_PL			16
#define DMA_CR_DBM			18
#define DMA_CR_CT			19
#define DMA_CR_PBURST		21
#define DMA_CR_MBURST		23
#define DMA_CR_CHSEL		25

#define DMA_CHANNEL_0		0x0
#define DMA_CHANNEL_1		0x1
#define DMA_CHANNEL_2		0x2
#define DMA_CHANNEL_3		0x3
#define DMA_CHANNEL_4		0x4
#define DMA_CHANNEL_5		0x5
#define DMA_CHANNEL_6		0x6
#define DMA_CHANNEL_7		0x7

#define DMA_PRIORITY_LEVEL_LOW			0x0
#define DMA_PRIORITY_LEVEL_MEDIUM		0x1
#define DMA_PRIORITY_LEVEL_HIGH			0x2
#define DMA_PRIORITY_LEVEL_VERY_HIGH	0x3

#define DMA_DATA_SIZE_BYTE				0x0
#define DMA_DATA_SIZE_HALF_WORD			0x1
#define DMA_DATA_SIZE_WORD				0x2

#define DMA_ADDR_FIXED					0x0
#define DMA_ADDR_INCREMENT				0x1

#define DMA_DIRECTION_P2M				0x0
#define DMA_DIRECTION_M2P				0x1
#define DMA_DIRECTION_M2M				0x2

#define DMA_TCIE_ENABLE					0x1
#define DMA_TCIE_DISABLE				0x0

#define DMA_HTIE_ENABLE					0x1
#define DMA_HTIE_DISABLE				0x0

void DMA_Config(struct DMA_Handle_t *pDMA_Handle);
void DMA_PeripheralEnable(struct DMA_Handle_t *pDMA_Handle);
void DMA_PeripheralDisable(struct DMA_Handle_t *pDMA_Handle);

#endif /* INC_DMA_DRIVER_H_ */
