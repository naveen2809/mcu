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
#include <stdlib.h>
#include "common_utils.h"

#define TRUE	1
#define FALSE	0

extern void USART_RXNEInterruptCallback(uint8_t data);
extern void USART_RX_Interrupt_Application_Callback(uint8_t data);


static volatile char * usart_tx_message;
static volatile int usart_tx_message_len;
static volatile int usart_tx_count;
static volatile uint8_t usart_tx_begin;

static uint8_t usart_available = TRUE;

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

void USART_ReceiveCmd(struct USART_Handle_t *pUSART_Handle, uint8_t *pCmdBuf, uint32_t *len)
{
	uint8_t c;

	*len = 0;

	while(1)
	{
		while(!USART_GetFlagStatus(pUSART_Handle,USART_FLAG_RXNE));

		c = pUSART_Handle->pUSART->USART_DR;
		*pCmdBuf = c;
		pCmdBuf++;
		*len += 1;

		if(c == '\n' || c == '\r')
		{
			return;
		}

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

void USART_DisableTXEInterrupt(struct USART_Handle_t *pUSART_Handle)
{
	pUSART_Handle->pUSART->USART_CR1 &= ~(1 << USART_CR1_TXEIE);
}

void USART_SendData_Interrupt(char *pTxBuf, uint32_t Len, uint8_t usart_irq_num)
{

	//Synchronization mechanism to guard against successive calls to the this function
	//Ensures processing of first message is completed before taking up new message
	while(usart_available == FALSE);

	usart_available = FALSE;

	//Update the data buffers and enable the interrupt
	usart_tx_message = pTxBuf;
	usart_tx_message_len = Len;

	NVIC_EnableIRQ(usart_irq_num);
	usart_tx_count = 0;
	usart_tx_begin = TRUE;
	delay_us(5000);
	NVIC_IRQSetPending(usart_irq_num);

	return;
}

void USART_Tx_Interrupt_Callback(struct USART_Handle_t *pUSART_Handle)
{
	if(usart_tx_begin == TRUE)
	{
		USART_EnableTXEInterrupt(pUSART_Handle);
		usart_tx_begin = FALSE;
		return;
	}

	if(usart_tx_count < usart_tx_message_len)
	{
		pUSART_Handle->pUSART->USART_DR = usart_tx_message[usart_tx_count];
		usart_tx_count++;
	}

	if(usart_tx_count == usart_tx_message_len)
	{
		USART_DisableTXEInterrupt(pUSART_Handle);
		usart_available = TRUE;
	}

	return;
}

void USART_Rx_Interrupt_Callback(struct USART_Handle_t *pUSART_Handle)
{
	USART_RX_Interrupt_Application_Callback(pUSART_Handle->pUSART->USART_DR);
}
