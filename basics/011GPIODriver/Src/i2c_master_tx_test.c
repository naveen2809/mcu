/*
 * i2c_tx_test.c
 *
 *  Created on: 02-Aug-2023
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
#define GPIO_PIN_NUM_INPUT				GPIO_PIN_0
#define GPIO_PIN_NUM_OUTPUT_1			GPIO_PIN_14
#define GPIO_PIN_NUM_OUTPUT_2			GPIO_PIN_15
#define SLEEP_COUNT						500000

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{
	struct I2C_Config_t I2C_Config;

	char message[]="Hello World!!!\r\n";
	char message2[]="This is another message!\r\n";

	uint8_t SlaveAddress = 0x04;
	uint8_t MessageLen[2];
	uint8_t BtnPressCnt = 0;

	memset(&I2C_Config,0,sizeof(I2C_Config));

	//GPIO Pin Configuration for Button and LED
	EnablePeriClk(GPIOA);
	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOA,GPIO_PIN_NUM_INPUT,GPIO_MODE_INPUT);
	GPIOSetMode(GPIOD,GPIO_PIN_NUM_OUTPUT_1,GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD,GPIO_PIN_NUM_OUTPUT_2,GPIO_MODE_OUTPUT);

	//GPIO Pin Configuration for I2C
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

	I2CEnable(I2C1);

	while(1)
	{
		if(GPIOReadPin(GPIOA,GPIO_PIN_NUM_INPUT) == GPIO_HIGH)
		{
			BtnPressCnt++;
			if(BtnPressCnt%2 == 1)
			{
				MessageLen[0] = (uint8_t) strlen(message);
				MessageLen[1] = (uint8_t) strlen(message);
				I2CMasterSendData(I2C1,MessageLen,2, SlaveAddress);
				GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_1);
				delay();
				I2CMasterSendData(I2C1,(uint8_t *)message,strlen(message), SlaveAddress);
				GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_2);
				delay();
			}
			else
			{
				MessageLen[0] = (uint8_t) strlen(message2);
				MessageLen[1] = (uint8_t) strlen(message2);
				I2CMasterSendData(I2C1,MessageLen,2, SlaveAddress);
				GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_1);
				delay();
				I2CMasterSendData(I2C1,(uint8_t *)message2,strlen(message2), SlaveAddress);
				GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_2);
				delay();
			}
		}
	}

	I2CDisable(I2C1);
	DisablePeriClk(I2C1);

	return 0;
}
