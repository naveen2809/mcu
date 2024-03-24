/*
 * flash_driver.c
 *
 *  Created on: 21-Mar-2024
 *      Author: naveen
 */

#include "flash_driver.h"
#include <stdint.h>

struct FLASH_RegDef_t *FLASH;

void flash_sector_erase(uint32_t sector_num)
{
	FLASH = (struct FLASH_RegDef_t *)FLASH_BASE_ADDR;

	//Ensure that no FLASH Memory operation is ongoing by checking the BSY bit in status register
	while((FLASH->FLASH_SR & FLASH_SR_BSY));
	
	//Unlock the FLASH Memory
	flash_unlock();

	//Program the FLASH_CR register to perform the sector erase operation
	//a. Set the FLASH PSIZE to one byte
	FLASH->FLASH_CR &= ~(0x3 << FLASH_CR_PSIZE);

	//b. Configure sector erase operation
	FLASH->FLASH_CR |= (0x1 << FLASH_CR_SER);

	//c. Configure the sector number
	FLASH->FLASH_CR &= ~(0xF << FLASH_CR_SNB);
	FLASH->FLASH_CR |= (sector_num << FLASH_CR_SNB);

	//d. Start the sector erase operation
	FLASH->FLASH_CR |= (0x1 << FLASH_CR_STRT);

	//Check that the FLASH operation is completed
	while((FLASH->FLASH_SR & FLASH_SR_BSY));

	//Lock the FLASH Memory
	flash_lock();

	return;
}

void flash_mass_erase(void)
{
	struct FLASH_RegDef_t *FLASH = (struct FLASH_RegDef_t *)FLASH_BASE_ADDR;

	//Ensure that no FLASH Memory operation is ongoing by checking the BSY bit in status register
	while((FLASH->FLASH_SR & FLASH_SR_BSY));

	//Unlock the FLASH Memory
	flash_unlock();

	//Program the FLASH_CR register to perform the mass erase operation
	//a. Set the FLASH PSIZE to one byte
	FLASH->FLASH_CR &= ~(0x3 << FLASH_CR_PSIZE);

	//b. Configure mass erase operation
	FLASH->FLASH_CR |= (0x1 << FLASH_CR_MER);

	//c. Start the sector erase operation
	FLASH->FLASH_CR |= (0x1 << FLASH_CR_STRT);

	//Check that the FLASH operation is completed
	while((FLASH->FLASH_SR & FLASH_SR_BSY));

	//Lock the FLASH Memory
	flash_lock();

	return;
}

void flash_unlock(void)
{
	struct FLASH_RegDef_t *FLASH = (struct FLASH_RegDef_t *)FLASH_BASE_ADDR;

	FLASH->FLASH_KEYR = 0x45670123;
	FLASH->FLASH_KEYR = 0xCDEF89AB;

	return;
}

void flash_lock(void)
{
	struct FLASH_RegDef_t *FLASH = (struct FLASH_RegDef_t *)FLASH_BASE_ADDR;

	FLASH->FLASH_CR |= (1 << FLASH_CR_LOCK);

	return;
}

void flash_read(uint8_t *RxBuf,uint8_t *fptr,uint32_t len)
{
	uint32_t i;

	for(i=0;i<len;i++)
	{
		*RxBuf++ = *fptr++;
	}

	return;
}

void flash_write(uint8_t *TxBuf,uint8_t *fptr,uint32_t len)
{
	struct FLASH_RegDef_t *FLASH = (struct FLASH_RegDef_t *)FLASH_BASE_ADDR;
	uint32_t i;

	//Unlock the FLASH Memory
	flash_unlock();

	//Set the FLASH PSIZE to one byte
	FLASH->FLASH_CR &= ~(0x3 << FLASH_CR_PSIZE);

	for(i=0;i<len;i++)
	{
		//a. Ensure that no FLASH Memory operation is ongoing by checking the BSY bit in status register
		while((FLASH->FLASH_SR & FLASH_SR_BSY));

		//b. Set the PG bit in the FLASH_CR register
		FLASH->FLASH_CR |= (0x1 << FLASH_CR_PG);

		//c. Write the data to FLASH Memory
		*fptr++ = *TxBuf++;
	}

	//Wait for the BSY bit to be cleared
	while((FLASH->FLASH_SR & FLASH_SR_BSY));

	//Lock the FLASH Memory
	flash_lock();

	return;
}
