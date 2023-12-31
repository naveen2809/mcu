/*
 * dma_driver.c
 *
 *  Created on: Oct 26, 2023
 *      Author: naveen
 */

#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "dma_driver.h"
#include <stdio.h>

void DMA_Config(struct DMA_Handle_t *pDMA_Handle)
{
	//1. Configure the DMA Channel
	pDMA_Handle->pDMA_Stream->DMA_CR &= ~(0x7 << DMA_CR_CHSEL);
	pDMA_Handle->pDMA_Stream->DMA_CR |= (pDMA_Handle->DMA_Config.DMA_Channel << DMA_CR_CHSEL);

	//2. Configure the DMA Direction
	pDMA_Handle->pDMA_Stream->DMA_CR &= ~(0x3 << DMA_CR_DIR);
	pDMA_Handle->pDMA_Stream->DMA_CR |= (pDMA_Handle->DMA_Config.DMA_Data_Transfer_Direction << DMA_CR_DIR);

	//3. Configure the Memory Address
	pDMA_Handle->pDMA_Stream->DMA_M0AR = pDMA_Handle->DMA_Config.DMA_M0_Address;

	//4. Configure the Peripheral Address
	pDMA_Handle->pDMA_Stream->DMA_PAR = pDMA_Handle->DMA_Config.DMA_Peripheral_Address;

	//5. Configure the Memory Data Size
	pDMA_Handle->pDMA_Stream->DMA_CR &= ~(0x3 << DMA_CR_MSIZE);
	pDMA_Handle->pDMA_Stream->DMA_CR |= (pDMA_Handle->DMA_Config.DMA_Memory_Data_Size << DMA_CR_MSIZE);

	//6. Configure the Peripheral Data Size
	pDMA_Handle->pDMA_Stream->DMA_CR &= ~(0x3 << DMA_CR_PSIZE);
	pDMA_Handle->pDMA_Stream->DMA_CR |= (pDMA_Handle->DMA_Config.DMA_Peripheral_Data_Size << DMA_CR_PSIZE);

	//7. Configure the Memory Address Increment
	pDMA_Handle->pDMA_Stream->DMA_CR &= ~(0x1 << DMA_CR_MINC);
	pDMA_Handle->pDMA_Stream->DMA_CR |= (pDMA_Handle->DMA_Config.DMA_Memory_Address_Increment << DMA_CR_MINC);

	//8. Configure the Peripheral Address Increment

	pDMA_Handle->pDMA_Stream->DMA_CR &= ~(0x1 << DMA_CR_PINC);
	pDMA_Handle->pDMA_Stream->DMA_CR |= (pDMA_Handle->DMA_Config.DMA_Peripheral_Address_Increment << DMA_CR_PINC);

	//9. Configure the DMA Stream Priority
	pDMA_Handle->pDMA_Stream->DMA_CR &= ~(0x3 << DMA_CR_PL);
	pDMA_Handle->pDMA_Stream->DMA_CR |= (pDMA_Handle->DMA_Config.DMA_Stream_Priority_Level << DMA_CR_PL);

	//10. Configure the number of bytes to be transferred
	pDMA_Handle->pDMA_Stream->DMA_NDTR = pDMA_Handle->DMA_Config.DMA_Num_Bytes;

	//11. Configure the TCIE Status
	pDMA_Handle->pDMA_Stream->DMA_CR &= ~(0x1 << DMA_CR_TCIE);
	pDMA_Handle->pDMA_Stream->DMA_CR |= (pDMA_Handle->DMA_Config.DMA_TCIE_Status << DMA_CR_TCIE);

	//12. Configure the HTIE Status
	pDMA_Handle->pDMA_Stream->DMA_CR &= ~(0x1 << DMA_CR_HTIE);
	pDMA_Handle->pDMA_Stream->DMA_CR |= (pDMA_Handle->DMA_Config.DMA_HTIE_Status << DMA_CR_HTIE);

}

void DMA_PeripheralEnable(struct DMA_Handle_t *pDMA_Handle)
{
	pDMA_Handle->pDMA_Stream->DMA_CR |= (1 << DMA_CR_EN);
}

void DMA_PeripheralDisable(struct DMA_Handle_t *pDMA_Handle)
{
	pDMA_Handle->pDMA_Stream->DMA_CR &= ~(1 << DMA_CR_EN);
}
