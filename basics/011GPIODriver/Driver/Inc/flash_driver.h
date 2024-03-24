/*
 * flash_driver.h
 *
 *  Created on: 21-Mar-2024
 *      Author: naveen
 */

#ifndef INC_FLASH_DRIVER_H_
#define INC_FLASH_DRIVER_H_

#include "stm32f4xx.h"
#include <stdint.h>

struct FLASH_RegDef_t
{
	uint32_t FLASH_ACR;
	uint32_t FLASH_KEYR;
	uint32_t FLASH_OPTKEYR;
	uint32_t FLASH_SR;
	uint32_t FLASH_CR;
	uint32_t FLASH_OPTCR;
};

#define FLASH_ACR_LATENCY		0
#define FLASH_ACR_PRFTEN		8
#define FLASH_ACR_ICEN			9
#define FLASH_ACR_DCEN			10
#define FLASH_ACR_ICRST			11
#define FLASH_ACR_DCRST			12

#define FLASH_SR_EOP			0
#define FLASH_SR_OPERR			1
#define FLASH_SR_WRPERR			4
#define FLASH_SR_PGAERR			5
#define FLASH_SR_PGPERR			6
#define FLASH_SR_PGSERR			7
#define FLASH_SR_RDERR			8
#define FLASH_SR_BSY			16

#define FLASH_CR_PG				0
#define FLASH_CR_SER			1
#define FLASH_CR_MER			2
#define FLASH_CR_SNB			3
#define FLASH_CR_PSIZE			8
#define FLASH_CR_STRT			16
#define FLASH_CR_EOPIE			24
#define FLASH_CR_ERRIE			25
#define FLASH_CR_LOCK			31

#define FLASH_OPTCR_OPTLOCK		0
#define FLASH_OPTCR_OPTSTRT		1
#define FLASH_OPTCR_BOR_LEV		2
#define FLASH_OPTCR_WDG_SW		5
#define FLASH_OPTCR_nRST_STOP	6
#define FLASH_OPTCR_nRST_STDBY	7
#define FLASH_OPTCR_RDP			8
#define FLASH_OPTCR_nWRP		16
#define FLASH_OPTCR_SPRMOD		31

void flash_sector_erase(uint32_t sector_num);
void flash_mass_erase(void);
void flash_unlock(void);
void flash_lock(void);
void flash_read(uint8_t *RxBuf,uint8_t *fptr,uint32_t len);
void flash_write(uint8_t *TxBuf,uint8_t *fptr,uint32_t len);

#endif /* INC_FLASH_DRIVER_H_ */
