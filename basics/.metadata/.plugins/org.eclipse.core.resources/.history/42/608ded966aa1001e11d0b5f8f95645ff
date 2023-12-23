/*
 * i2c_test.c
 *
 *  Created on: 11-Jul-2023
 *      Author: naveen
 */


#include "stm32f4xx.h"
#include "i2c_driver.h"
#include "gpio_driver.h"
#include <stdint.h>
#include <string.h>

#define I2C_CONFIG_PERI_FREQ			0x10
#define I2C_CONFIG_CCR_VALUE			0x50
#define TRISE_VALUE						0x11
#define I2C1_SDA						GPIO_PIN_7
#define I2C1_SCL						GPIO_PIN_6

int main(void)
{
	struct I2C_Config_t I2C_Config;

	char message[]="H";

	uint8_t SlaveAddress = 0x04;

	memset(&I2C_Config,0,sizeof(I2C_Config));

	//GPIO Pin Configuration
	EnablePeriClk(GPIOB);
	GPIOSetMode(GPIOB,I2C1_SDA,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOB,I2C1_SCL,GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOB,I2C1_SDA,GPIO_ALTFN_4);
	GPIOSetAltFn(GPIOB,I2C1_SCL,GPIO_ALTFN_4);
	GPIOSetOutputType(GPIOB,I2C1_SDA,GPIO_OPTYPE_OD);
	GPIOSetOutputType(GPIOB,I2C1_SCL,GPIO_OPTYPE_OD);
	GPIOSetOutputSpeed(GPIOB,I2C1_SDA,GPIO_OPSPEED_HIGH);
	GPIOSetOutputSpeed(GPIOB,I2C1_SCL,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(GPIOB,I2C1_SDA,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(GPIOB,I2C1_SCL,GPIO_PULL_UP);

	//I2C Port Configuration
	I2C_Config.I2C_PeriFreq = I2C_CONFIG_PERI_FREQ;
	I2C_Config.I2C_SpeedMode = I2C_MODE_SM;
	I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_0;
	I2C_Config.I2C_CCRValue = I2C_CONFIG_CCR_VALUE;
	I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;

	EnablePeriClk(I2C1);

	I2CPeriConfig(I2C1,&I2C_Config);
	I2CConfigureTrise(I2C1,TRISE_VALUE);
	I2CConfigureSlaveAddress(I2C1,SlaveAddress);

	I2CEnable(I2C1);
	I2CEnableAck(I2C1);

	I2CMasterSendData(I2C1,(uint8_t *)message,1, SlaveAddress);
	I2CSlaveReceiveData(I2C1,(uint8_t *)message,1);

	I2CDisable(I2C1);

	DisablePeriClk(I2C1);

	while(1);

	return 0;
}
