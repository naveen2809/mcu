/*
 * usart_driver.h
 *
 *  Created on: 30-Jul-2023
 *      Author: naveen
 */

#ifndef INC_USART_DRIVER_H_
#define INC_USART_DRIVER_H_

#include "stm32f4xx.h"
#include <stdint.h>

struct USART_RegDef_t
{
	uint32_t USART_SR;
	uint32_t USART_DR;
	uint32_t USART_BRR;
	uint32_t USART_CR1;
	uint32_t USART_CR2;
	uint32_t USART_CR3;
	uint32_t USART_GTPR;
};

struct USART_Config_t
{
	uint32_t USART_Mode;
	uint32_t USART_DataLength;
	uint32_t USART_StopBits;
	uint32_t USART_ParityControl;
	uint32_t USART_BaudRate;
	uint32_t USART_HWFlowControl;
};

struct USART_Handle_t
{
	struct USART_RegDef_t *pUSART;
	struct USART_Config_t USART_Config;
};

#define USART_SR_PE			0
#define USART_SR_FE			1
#define USART_SR_NF			2
#define USART_SR_ORE		3
#define USART_SR_IDLE		4
#define USART_SR_RXNE		5
#define USART_SR_TC			6
#define USART_SR_TXE		7
#define USART_SR_LBD		8
#define USART_SR_CTS		9

#define USART_CR1_SBK		0
#define USART_CR1_RWU		1
#define USART_CR1_RE		2
#define USART_CR1_TE		3
#define USART_CR1_IDLEIE	4
#define USART_CR1_RXNEIE	5
#define USART_CR1_TCIE		6
#define USART_CR1_TXEIE		7
#define USART_CR1_PEIE		8
#define USART_CR1_PS		9
#define USART_CR1_PCE		10
#define USART_CR1_WAKE		11
#define USART_CR1_M			12
#define USART_CR1_UE		13
#define USART_CR1_OVER8		15

#define USART_CR2_ADD		0
#define USART_CR2_LBDL		5
#define USART_CR2_LBDIE		6
#define USART_CR2_LBCL		8
#define USART_CR2_CPHA		9
#define USART_CR2_CPOL		10
#define USART_CR2_CLKEN		11
#define USART_CR2_STOP		12
#define USART_CR2_LINEN		14

#define USART_CR3_EIE		0
#define USART_CR3_IREN		1
#define USART_CR3_IRLP		2
#define USART_CR3_HDSEL		3
#define USART_CR3_NACK		4
#define USART_CR3_SCEN		5
#define USART_CR3_DMAR		6
#define USART_CR3_DMAT		7
#define USART_CR3_RTSE		8
#define USART_CR3_CTSE		9
#define USART_CR3_CTSIE		10
#define USART_CR3_ONEBIT	11

#define USART_MODE_TX_ONLY		0
#define USART_MODE_RX_ONLY		1
#define USART_MODE_TX_RX		2

#define USART_DATA_LEN_8_BITS	0
#define USART_DATA_LEN_9_BITS	1

#define USART_STOP_BITS_1		0
#define USART_STOP_BITS_0_5		1
#define USART_STOP_BITS_2		2
#define USART_STOP_BITS_1_5		3

#define USART_PARITY_DISABLE	0
#define USART_PARITY_EVEN		1
#define USART_PARITY_ODD		2

#define USART_SB_RATE_1200		1200
#define USART_SB_RATE_2400		2400
#define USART_SB_RATE_9600		9600
#define USART_SB_RATE_19200		19200
#define USART_SB_RATE_38400		38400
#define USART_SB_RATE_57600		57600
#define USART_SB_RATE_115200	115200
#define USART_SB_RATE_230400	230400
#define USART_SB_RATE_460800	460800
#define USART_SB_RATE_921600	921600
#define USART_SB_RATE_2M		2000000
#define USART_SB_RATE_3M		3000000

#define USART_HW_FLOW_CNTRL_NONE		0
#define USART_HW_FLOW_CNTRL_RTS			1
#define USART_HW_FLOW_CNTRL_CTS			2
#define USART_HW_FLOW_CNTRL_RTS_CTS		3

#define USART_FLAG_TXE			(1 << USART_SR_TXE)
#define USART_FLAG_TC			(1 << USART_SR_TC)
#define USART_FLAG_RXNE			(1 << USART_SR_RXNE)

void USART_Init(struct USART_Handle_t *pUSART_Handle);
void USART_PeripheralEnable(struct USART_Handle_t *pUSART_Handle);
void USART_PeripheralDisable(struct USART_Handle_t *pUSART_Handle);
uint8_t USART_GetFlagStatus(struct USART_Handle_t *pUSART_Handle, uint8_t FlagName);
void USART_ClearFlag(struct USART_Handle_t *pUSART_Handle, uint8_t FlagName);
void USART_SetBaudRate(struct USART_Handle_t *pUSART_Handle, uint32_t BaudRate);

void USART_SendData(struct USART_Handle_t *pUSART_Handle, uint8_t *pTxBuf, uint32_t Len);
void USART_ReceiveData(struct USART_Handle_t *pUSART_Handle, uint8_t *pRxBuf, uint32_t Len);
uint8_t USART_SendDataIT(struct USART_Handle_t *pUSART_Handle, uint8_t *pTxBuf, uint32_t Len);
uint8_t USART_ReceiveDataIT(struct USART_Handle_t *pUSART_Handle, uint8_t *pRxBuf, uint32_t Len);

void USART_IRQHandling(struct USART_Handle_t *pUSART_Handle);
void USART_RXNEInterruptCallback(uint8_t data);
void USART_EnableRXNEInterrupt(struct USART_Handle_t *pUSART_Handle);
void USART_EnableTXEInterrupt(struct USART_Handle_t *pUSART_Handle);
void USART_DisableTXEInterrupt(struct USART_Handle_t *pUSART_Handle);
void USART_SendData_Interrupt(char *pTxBuf, uint32_t Len, uint8_t usart_irq_num);
void USART_Tx_Interrupt_Callback(struct USART_Handle_t *pUSART_Handle);
void USART_Rx_Interrupt_Callback(struct USART_Handle_t *pUSART_Handle);

void USART_ReceiveCmd(struct USART_Handle_t *pUSART_Handle, uint8_t *pCmdBuf, uint32_t *len);

#endif /* INC_USART_DRIVER_H_ */
