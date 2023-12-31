/*
 * i2c_driver.c
 *
 *  Created on: 10-Jul-2023
 *      Author: naveen
 */


#include "stm32f4xx.h"
#include "i2c_driver.h"
#include "gpio_driver.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void I2C_SendStartBit(uint32_t *I2CAddress);
static void I2C_SendStopBit(uint32_t *I2CAddress);
static uint8_t I2C_GetFlagStatus(uint32_t *I2CAddress, uint32_t Flag);
static void I2C_SendSlaveAddress(uint32_t *I2CAddress, uint8_t SlaveAddress);
static void I2C_ClearADDRStatusBit(uint32_t *I2CAddress);
static void I2C_ClearSTOPFStatusBit(uint32_t *I2CAddress);
static void I2C_ClearAFStatusBit(uint32_t *I2CAddress);

void I2CPeriConfig(uint32_t *I2CAddress, struct I2C_Config_t *pI2CConfig)
{

	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	//Configuring the I2C Peripheral Frequency
	pI2C->I2C_CR2 &= ~(0x3F << I2C_CR2_FREQ);
	pI2C->I2C_CR2 |= (pI2CConfig->I2C_PeriFreq << I2C_CR2_FREQ);

	//Configuring the I2C Speed Mode
	pI2C->I2C_CCR &= ~(1 << I2C_CCR_FS);
	pI2C->I2C_CCR |= (pI2CConfig->I2C_SpeedMode << I2C_CCR_FS);

	//Configuring the I2C FM Duty Cycle
	if(pI2CConfig->I2C_SpeedMode == I2C_MODE_FM)
	{
		pI2C->I2C_CCR &= ~(1 << I2C_CCR_DUTY);
		pI2C->I2C_CCR |= (pI2CConfig->I2C_FMDutyCycle << I2C_CCR_DUTY);
	}

	//Configuring the I2C CCR Value
	pI2C->I2C_CCR &= ~(0x0FFF << I2C_CCR_CCR);
	pI2C->I2C_CCR |= (pI2CConfig->I2C_CCRValue << I2C_CCR_CCR);

	//Configuring the ACK Value
	pI2C->I2C_CR1 &= ~(1 << I2C_CR1_ACK);
	pI2C->I2C_CR1 |= (pI2CConfig->I2C_AckControl << I2C_CR1_ACK);

}

void I2CEnable(uint32_t *I2CAddress)
{

	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	pI2C->I2C_CR1 |= (1 << I2C_CR1_PE);
}

void I2CDisable(uint32_t *I2CAddress)
{

	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	pI2C->I2C_CR1 &= ~(1 << I2C_CR1_PE);
}

void I2CMasterSendData(uint32_t *I2CAddress,uint8_t *TxBuf,uint32_t Len, uint8_t SlaveAddress)
{
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	//1. Generate Start Condition
	I2C_SendStartBit(I2CAddress);

	//2. Check for Successful Transmission of Start Condition using SB Flag
	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_SB));

	//3. Send Slave Address Address Along with  Write Bit
	SlaveAddress = SlaveAddress << 1;
	SlaveAddress &= ~(0x1);

	I2C_SendSlaveAddress(I2CAddress,SlaveAddress);

	//4. Check for Successful Completion of Address Phase (ACK received from slave)
	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_ADDR));

	I2C_ClearADDRStatusBit(I2CAddress);

	//5. Send Data While Len > 0

	while(Len > 0)
	{
		while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_TXE));
		pI2C->I2C_DR = *TxBuf;
		TxBuf++;
		Len--;
	}

	//6. Wait for last byte transmission to be completed
	// Check for TXE=1 and BTF=1

	while(I2C_GetFlagStatus(I2CAddress,I2C_FLAG_TXE)!=1 || I2C_GetFlagStatus(I2CAddress,I2C_FLAG_BTF)!=1);

	//7. Send Stop Bit
	I2C_SendStopBit(I2CAddress);

}

void I2CMasterReceiveData(uint32_t *I2CAddress,uint8_t *RxBuf,uint32_t Len,uint8_t SlaveAddress)
{

	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	//1. Generate Start Condition
	I2C_SendStartBit(I2CAddress);

	//2. Check for Successful Transmission of Start Condition using SB Flag
	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_SB));

	//3. Send Slave Address Address Along with Read Bit Set
	SlaveAddress = SlaveAddress << 1;
	SlaveAddress |= (0x1);

	I2C_SendSlaveAddress(I2CAddress,SlaveAddress);

	//4. Check for Successful Completion of Address Phase (ACK received from slave)
	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_ADDR));

	I2C_ClearADDRStatusBit(I2CAddress);

	//5. Receive Data While Len >=2 with ACK Bit Set
	if(Len >= 2)
	{
		I2CEnableAck(I2CAddress);
		while(Len >= 2)
		{
			while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_RXNE));
			*RxBuf = pI2C->I2C_DR;
			RxBuf++;
			Len--;
		}
	}

	//6. Receive Last Byte with NACK Bit Set
	if(Len == 1)
	{
		I2CDisableAck(I2CAddress);
		while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_RXNE));
		*RxBuf = pI2C->I2C_DR;
		RxBuf++;
		Len--;
	}

	//7. Send Stop Bit
	I2C_SendStopBit(I2CAddress);
}

void I2CSlaveSendData(uint32_t *I2CAddress,uint8_t *TxBuf,uint32_t Len)
{
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	//1. Check for ADDR Flag
	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_ADDR));

	//2. Clear ADDR Flag
	I2C_ClearADDRStatusBit(I2CAddress);

	//3. Send Data While Len > 0

	while(Len > 0)
	{
		while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_TXE));
		pI2C->I2C_DR = *TxBuf;
		TxBuf++;
		Len--;
	}

	//4. Check for Acknowledge Failure (AF) Flag which is set when NACK is received for last byte
	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_AF));

	//5. Acknowledge Failure (AF) Flag
	I2C_ClearAFStatusBit(I2CAddress);

}

void I2CSlaveReceiveData(uint32_t *I2CAddress,uint8_t *RxBuf,uint32_t Len)
{
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;
	int i;
	uint8_t *ptr=RxBuf;

	//1. Check for ADDR Flag
	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_ADDR));

	//2. Clear ADDR Flag
	I2C_ClearADDRStatusBit(I2CAddress);

	//3. Check for RXNE Flag and Receive Data

	while(Len > 0)
	{
		while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_RXNE));
		*RxBuf = pI2C->I2C_DR;
		RxBuf++;
		Len--;
	}

	//4. Check for Stop Condition

	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_STOPF));

	//5. Clear STOPF Flag
	I2C_ClearSTOPFStatusBit(I2CAddress);


	//6. Display the data
	for(i=0; i<Len; i++)
	{
		printf("%c",*ptr);
		ptr++;
	}

	printf("\r\n");

}

static void I2C_SendStartBit(uint32_t *I2CAddress)
{
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	pI2C->I2C_CR1 |= (1 << I2C_CR1_START);
}

static void I2C_SendStopBit(uint32_t *I2CAddress)
{
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	pI2C->I2C_CR1 |= (1 << I2C_CR1_STOP);
}

static uint8_t I2C_GetFlagStatus(uint32_t *I2CAddress, uint32_t Flag)
{
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	if(pI2C->I2C_SR1 & Flag)
	{
			return FLAG_SET;
	}

	return FLAG_RESET;
}

static void I2C_SendSlaveAddress(uint32_t *I2CAddress, uint8_t SlaveAddress)
{
	//uint32_t status;

	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	//status = pI2C->I2C_SR1;

	pI2C->I2C_DR = SlaveAddress;

	//(void)status;
}

static void I2C_ClearADDRStatusBit(uint32_t *I2CAddress)
{
	uint32_t status;
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	status = pI2C->I2C_SR1;
	status = pI2C->I2C_SR2;

	(void)status;
}

static void I2C_ClearSTOPFStatusBit(uint32_t *I2CAddress)
{
	uint32_t status;
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	status = pI2C->I2C_SR1;
	pI2C->I2C_CR1 |= 0x0;

	(void)status;
}

static void I2C_ClearAFStatusBit(uint32_t *I2CAddress)
{
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	pI2C->I2C_SR1 &= ~(1 << I2C_SR1_AF);
}

void I2CConfigureTrise(uint32_t *I2CAddress,uint8_t TriseVal)
{
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	pI2C->I2C_TRISE = TriseVal;
}

void I2CConfigureSlaveAddress(uint32_t *I2CAddress,uint8_t SlaveAddress)
{
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	uint16_t temp_address = (uint16_t) SlaveAddress;

	temp_address = (temp_address << 1);
	temp_address &= 0x00FE;

	pI2C->I2C_OAR1 = temp_address;
	pI2C->I2C_OAR1 &= ~(1 << 15);
	pI2C->I2C_OAR1 |= (1 << 14);
}

void I2CEnableAck(uint32_t *I2CAddress)
{
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	pI2C->I2C_CR1 |= (1 << I2C_CR1_ACK);
}

void I2CDisableAck(uint32_t *I2CAddress)
{
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	pI2C->I2C_CR1 &= ~(1 << I2C_CR1_ACK);
}

void I2CMaster_AT24Cxx_EEPROM_Read_Sequential(uint32_t *I2CAddress,uint8_t *RxBuf,uint32_t Len,uint16_t eeprom_address, uint8_t SlaveAddress)
{
	uint8_t address_length,i;
	uint8_t data[2];
	uint8_t LocalSlaveAddress;

	address_length = 2;
	data[0] = (uint8_t) (eeprom_address >> 8);
	data[1] = (uint8_t) (eeprom_address & 0xFF);

	//Send the Address Bytes to EEPROM
	struct I2C_RegDef_t *pI2C = (struct I2C_RegDef_t *) I2CAddress;

	//1. Generate Start Condition
	I2C_SendStartBit(I2CAddress);

	//2. Check for Successful Transmission of Start Condition using SB Flag
	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_SB));

	//3. Send Slave Address Address Along with  Write Bit
	LocalSlaveAddress = SlaveAddress << 1;
	LocalSlaveAddress &= ~(0x1);

	I2C_SendSlaveAddress(I2CAddress,LocalSlaveAddress);

	//4. Check for Successful Completion of Address Phase (ACK received from slave)
	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_ADDR));

	I2C_ClearADDRStatusBit(I2CAddress);


	//5. Send EEPROM Address Bytes

	i=0;
	while(address_length > 0)
	{
		while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_TXE));
		pI2C->I2C_DR = data[i];
		i++;
		address_length--;
	}

	//6. Wait for last byte transmission to be completed
	// Check for TXE=1 and BTF=1

	while(I2C_GetFlagStatus(I2CAddress,I2C_FLAG_TXE)!=1 || I2C_GetFlagStatus(I2CAddress,I2C_FLAG_BTF)!=1);

	//Read from EEPROM

	//1. Generate Start Condition
	I2C_SendStartBit(I2CAddress);

	//2. Check for Successful Transmission of Start Condition using SB Flag
	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_SB));

	//3. Send Slave Address Address Along with Read Bit Set
	LocalSlaveAddress = SlaveAddress << 1;
	LocalSlaveAddress |= (0x1);

	I2C_SendSlaveAddress(I2CAddress,LocalSlaveAddress);

	//4. Check for Successful Completion of Address Phase (ACK received from slave)
	while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_ADDR));

	I2C_ClearADDRStatusBit(I2CAddress);

	//5. Receive Data While Len >=2 with ACK Bit Set
	if(Len >= 2)
	{
		I2CEnableAck(I2CAddress);
		while(Len >= 2)
		{
			while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_RXNE));
			*RxBuf = pI2C->I2C_DR;
			RxBuf++;
			Len--;
		}
	}

	//6. Receive Last Byte with NACK Bit Set
	if(Len == 1)
	{
		I2CDisableAck(I2CAddress);
		while(! I2C_GetFlagStatus(I2CAddress,I2C_FLAG_RXNE));
		*RxBuf = pI2C->I2C_DR;
		RxBuf++;
		Len--;
	}

	//7. Send Stop Bit
	I2C_SendStopBit(I2CAddress);

	return;
}
