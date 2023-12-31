/*
 * usart_driver.c
 *
 *  Created on: 30-Jul-2023
 *      Author: naveen
 */


#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "usart_driver.h"
#include <stdio.h>

extern void USART_RXNEInterruptCallback(uint8_t data);

void USART_Init(struct USART_Handle_t *pUSART_Handle)
{
	//1. Configure Tx/Rx Mode

	if(pUSART_Handle->USART_Config.USART_Mode == USART_MODE_TX_ONLY)
	{
		pUSART_Handle->pUSART->USART_CR1 |= (1 << USART_CR1_TE);
		pUSART_Handle->pUSART->USART_CR1 &= ~(1 << USART_CR1_RE);
	}else if(pUSART_Handle->USART_Config.USART_Mode == USART_MODE_RX_ONLY)
	{
		pUSART_Handle->pUSART->USART_CR1 &= ~(1 << USART_CR1_TE);
		pUSART_Handle->pUSART->USART_CR1 |= (1 << USART_CR1_RE);
	}else if(pUSART_Handle->USART_Config.USART_Mode == USART_MODE_TX_RX)
	{
		pUSART_Handle->pUSART->USART_CR1 |= (1 << USART_CR1_TE);
		pUSART_Handle->pUSART->USART_CR1 |= (1 << USART_CR1_RE);
	}


	//2. Configure Data Word Length

	if(pUSART_Handle->USART_Config.USART_DataLength == USART_DATA_LEN_8_BITS)
	{
		pUSART_Handle->pUSART->USART_CR1 &= ~(1 << USART_CR1_M);
	}else if(pUSART_Handle->USART_Config.USART_DataLength == USART_DATA_LEN_9_BITS)
	{
		pUSART_Handle->pUSART->USART_CR1 |= (1 << USART_CR1_M);
	}


	//3. Configure the No. of Stop Bits

	pUSART_Handle->pUSART->USART_CR2 &= ~(0x3 << USART_CR2_STOP);
	pUSART_Handle->pUSART->USART_CR2 |= (pUSART_Handle->USART_Config.USART_StopBits << USART_CR2_STOP);

	//4. Configure the Parity

	if(pUSART_Handle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
	{
		pUSART_Handle->pUSART->USART_CR1 &= ~(1 << USART_CR1_PCE);
	}else if(pUSART_Handle->USART_Config.USART_ParityControl == USART_PARITY_EVEN)
	{
		pUSART_Handle->pUSART->USART_CR1 |= (1 << USART_CR1_PCE);
		pUSART_Handle->pUSART->USART_CR1 &= ~(1 << USART_CR1_PS);
	}else if(pUSART_Handle->USART_Config.USART_ParityControl == USART_PARITY_ODD)
	{
		pUSART_Handle->pUSART->USART_CR1 |= (1 << USART_CR1_PCE);
		pUSART_Handle->pUSART->USART_CR1 |= (1 << USART_CR1_PS);
	}

	//5. Configure the Baud Rate
	USART_SetBaudRate(pUSART_Handle, pUSART_Handle->USART_Config.USART_BaudRate);

	//6. Configure HW Flow Control
	if(pUSART_Handle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CNTRL_NONE)
	{
		pUSART_Handle->pUSART->USART_CR3 &= ~(1 << USART_CR3_RTSE);
		pUSART_Handle->pUSART->USART_CR3 &= ~(1 << USART_CR3_CTSE);
	}else if(pUSART_Handle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CNTRL_RTS)
	{
		pUSART_Handle->pUSART->USART_CR3 |= (1 << USART_CR3_RTSE);
		pUSART_Handle->pUSART->USART_CR3 &= ~(1 << USART_CR3_CTSE);
	}else if(pUSART_Handle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CNTRL_CTS)
	{
		pUSART_Handle->pUSART->USART_CR3 &= ~(1 << USART_CR3_RTSE);
		pUSART_Handle->pUSART->USART_CR3 |= (1 << USART_CR3_CTSE);
	}else if(pUSART_Handle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CNTRL_RTS_CTS)
	{
		pUSART_Handle->pUSART->USART_CR3 |= (1 << USART_CR3_RTSE);
		pUSART_Handle->pUSART->USART_CR3 |= (1 << USART_CR3_CTSE);
	}

}

void USART_PeripheralEnable(struct USART_Handle_t *pUSART_Handle)
{
	pUSART_Handle->pUSART->USART_CR1 |= (1 << USART_CR1_UE);
}

void USART_PeripheralDisable(struct USART_Handle_t *pUSART_Handle)
{
	pUSART_Handle->pUSART->USART_CR1 &= ~(1 << USART_CR1_UE);
}

uint8_t USART_GetFlagStatus(struct USART_Handle_t *pUSART_Handle, uint8_t FlagName)
{
	if(pUSART_Handle->pUSART->USART_SR & FlagName)
	{
		return FLAG_SET;
	}

		return FLAG_RESET;
}

void USART_ClearFlag(struct USART_Handle_t *pUSART_Handle, uint8_t FlagName)
{

}


void USART_SendData(struct USART_Handle_t *pUSART_Handle, uint8_t *pTxBuf, uint32_t Len)
{
	while(Len > 0)
	{
		while(!USART_GetFlagStatus(pUSART_Handle,USART_FLAG_TXE));

		pUSART_Handle->pUSART->USART_DR = *pTxBuf;
		pTxBuf++;
		Len--;
	}

	while(!USART_GetFlagStatus(pUSART_Handle,USART_FLAG_TC));
}

void USART_ReceiveData(struct USART_Handle_t *pUSART_Handle, uint8_t *pRxBuf, uint32_t Len)
{
	while(Len > 0)
	{
		while(!USART_GetFlagStatus(pUSART_Handle,USART_FLAG_RXNE));

		*pRxBuf = pUSART_Handle->pUSART->USART_DR;
		pRxBuf++;
		Len--;
	}
}

void USART_SetBaudRate(struct USART_Handle_t *pUSART_Handle, uint32_t BaudRate)
{

	// Dummy Function. Need to implement code to calculate the USART_BRR_Value
	//0x683 corresponds to FPCLK=16MHz and Oversampling=0
	uint32_t USART_BRR_Value = 0x683;

	pUSART_Handle->pUSART->USART_BRR = USART_BRR_Value;
}

void USART_IRQHandling(struct USART_Handle_t *pUSART_Handle)
{

	//Clear TC Flag

	//Code for handling RXNE interrupt

	if(USART_GetFlagStatus(pUSART_Handle,USART_FLAG_RXNE))
	{
		USART_RXNEInterruptCallback(pUSART_Handle->pUSART->USART_DR);
	}

}

void USART_EnableRXNEInterrupt(struct USART_Handle_t *pUSART_Handle)
{
	pUSART_Handle->pUSART->USART_CR1 |= (1 << USART_CR1_RXNEIE);
}

void USART_EnableTXEInterrupt(struct USART_Handle_t *pUSART_Handle)
{
	pUSART_Handle->pUSART->USART_CR1 |= (1 << USART_CR1_TXEIE);
}
