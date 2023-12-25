/*
 * i2c_driver.h
 *
 *  Created on: 10-Jul-2023
 *      Author: naveen
 */

#ifndef INC_I2C_DRIVER_H_
#define INC_I2C_DRIVER_H_

#include "stm32f4xx.h"
#include <stdint.h>

struct I2C_RegDef_t
{
	uint32_t I2C_CR1;
	uint32_t I2C_CR2;
	uint32_t I2C_OAR1;
	uint32_t I2C_OAR2;
	uint32_t I2C_DR;
	uint32_t I2C_SR1;
	uint32_t I2C_SR2;
	uint32_t I2C_CCR;
	uint32_t I2C_TRISE;
	uint32_t I2C_FLTR;
};

struct I2C_Config_t
{
	uint32_t I2C_PeriFreq;
	uint32_t I2C_CCRValue;
	uint32_t I2C_SpeedMode;
	uint32_t I2C_FMDutyCycle;
	uint32_t I2C_AckControl;
	uint32_t I2C_DeviceAddress;          //Slave Mode Device Address
};

#define I2C_CR1_PE				0
#define I2C_CR1_SMBUS			1
#define I2C_CR1_SMBTYPE			3
#define I2C_CR1_ENARP			4
#define I2C_CR1_ENPEC			5
#define I2C_CR1_ENGC			6
#define I2C_CR1_NOSTRETCH		7
#define I2C_CR1_START			8
#define I2C_CR1_STOP			9
#define I2C_CR1_ACK				10
#define I2C_CR1_POS				11
#define I2C_CR1_PEC				12
#define I2C_CR1_ALERT			13
#define I2C_CR1_SWRST			15

#define I2C_CR2_FREQ			0
#define I2C_CR2_ITERREN			8
#define I2C_CR2_ITEVTEN			9
#define I2C_CR2_ITBUFEN			10
#define I2C_CR2_DMAEN			11
#define I2C_CR2_LAST			12

#define I2C_SR1_SB				0
#define I2C_SR1_ADDR			1
#define I2C_SR1_BTF				2
#define I2C_SR1_ADD10			3
#define I2C_SR1_STOPF			4
#define I2C_SR1_RxNE			6
#define I2C_SR1_TxE				7
#define I2C_SR1_BERR			8
#define I2C_SR1_ARLO			9
#define I2C_SR1_AF				10
#define I2C_SR1_OVR				11
#define I2C_SR1_PECERR			12
#define I2C_SR1_TIMEOUT			14
#define I2C_SR1_SMBALERT		15

#define I2C_SR2_MSL				0
#define I2C_SR2_BUSY			1
#define I2C_SR2_TRA				2
#define I2C_SR2_GENCALL			4
#define I2C_SR2_SMBDEFAULT		5
#define I2C_SR2_SMBHOST			6
#define I2C_SR2_DUALF			7
#define I2C_SR2_PEC				8

#define I2C_CCR_CCR				0
#define I2C_CCR_DUTY			14
#define I2C_CCR_FS				15

#define I2C_MODE_SM				0
#define I2C_MODE_FM				1
#define I2C_FM_DUTY_0			0
#define I2C_FM_DUTY_1			1
#define I2C_ACK_DISABLE			0
#define I2C_ACK_ENABLE			1

#define I2C_FLAG_SB				(1 << I2C_SR1_SB)
#define I2C_FLAG_ADDR			(1 << I2C_SR1_ADDR)
#define I2C_FLAG_BTF			(1 << I2C_SR1_BTF)
#define I2C_FLAG_TXE			(1 << I2C_SR1_TxE)
#define I2C_FLAG_RXNE			(1 << I2C_SR1_RxNE)
#define I2C_FLAG_STOPF			(1 << I2C_SR1_STOPF)
#define I2C_FLAG_AF				(1 << I2C_SR1_AF)

void I2CPeriConfig(uint32_t *I2CAddress, struct I2C_Config_t *pI2CConfig);
void I2CEnable(uint32_t *I2CAddress);
void I2CDisable(uint32_t *I2CAddress);
void I2CConfigureTrise(uint32_t *I2CAddress,uint8_t TriseVal);
void I2CConfigureSlaveAddress(uint32_t *I2CAddress,uint8_t SlaveAddress);
void I2CEnableAck(uint32_t *I2CAddress);
void I2CDisableAck(uint32_t *I2CAddress);

void I2CMasterSendData(uint32_t *I2CAddress,uint8_t *TxBuf,uint32_t Len,uint8_t SlaveAddress);
void I2CMasterReceiveData(uint32_t *I2CAddress,uint8_t *RxBuf,uint32_t Len,uint8_t SlaveAddress);
void I2CSlaveSendData(uint32_t *I2CAddress,uint8_t *TxBuf,uint32_t Len);
void I2CSlaveReceiveData(uint32_t *I2CAddress,uint8_t *RxBuf,uint32_t Len);

void I2CMaster_ATC24xx_EEPROM_Read_Sequential(uint32_t *I2CAddress,uint8_t *RxBuf,uint32_t Len,uint16_t eeprom_address, uint8_t SlaveAddress);

#endif /* INC_I2C_DRIVER_H_ */
