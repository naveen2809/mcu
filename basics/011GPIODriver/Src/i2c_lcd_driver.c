/*
 * i2c_lcd_driver.c
 *
 *  Created on: 13-Dec-2023
 *      Author: naveen
 */


#include "stm32f4xx.h"
#include "i2c_driver.h"
#include "gpio_driver.h"
#include <stdint.h>
#include <string.h>
#include "common_utilities.h"

#define I2C_CONFIG_PERI_FREQ			0x28
#define I2C_CONFIG_CCR_VALUE			0x04
#define TRISE_VALUE						0x0D
#define I2C1_SDA						GPIO_PIN_9
#define I2C1_SCL						GPIO_PIN_8

#define RCC_CR_REG_ADDR					0x40023800UL
#define RCC_CFGR_REG_ADDR				0x40023808UL
#define RCC_PLLCFGR_REG_ADDR			0x40023804UL
#define	FLASH_ACR_REG_ADDR				0x40023C00UL

#define PLL_SRC_HSI						0
#define PLL_SRC_HSE						1
#define PLL_M_BITPOS					0
#define PLL_N_BITPOS					6
#define PLL_P_BITPOS					16
#define PLL_Q_BITPOS					24
#define PLL_SRC_BITPOS					22

#define HSE_ON_BITPOS					16
#define HSE_CHECK_BITPOS				17
#define PLL_ON_BITPOS					24
#define PLL_CHECK_BITPOS				25

#define SYSCLK_SW_HSI					0
#define SYSCLK_SW_HSE					1
#define SYSCLK_SW_PLL					2
#define SYSCLK_SW_BITPOS				0

#define RCC_CFGR_PPRE1_BITPOS			10
#define RCC_CFGR_PPRE2_BITPOS			13

struct I2C_Config_t I2C_Config;
uint8_t SlaveAddress = 0x3F;

void i2c_lcd_init(void);
void i2c_lcd_send_char(uint8_t c);
void i2c_lcd_send_message(char *message,uint8_t length);
void config_system_clock(void);

int main(void)
{
	uint8_t ch = 0x20;

	char message[] = "Hello World!";

	//Configure the System Clock
	config_system_clock();

	//Configure the Timer
	configure_delay_timer();

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
	memset(&I2C_Config,0,sizeof(I2C_Config));
	I2C_Config.I2C_PeriFreq = I2C_CONFIG_PERI_FREQ;
	I2C_Config.I2C_SpeedMode = I2C_MODE_FM;
	I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_1;
	I2C_Config.I2C_CCRValue = I2C_CONFIG_CCR_VALUE;
	I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;

	EnablePeriClk(I2C1);

	I2CPeriConfig(I2C1,&I2C_Config);
	I2CConfigureTrise(I2C1,TRISE_VALUE);

	I2CEnable(I2C1);

	i2c_lcd_init();

	i2c_lcd_send_message(message,strlen(message));


	delay_us(5000);
	i2c_lcd_send_char(ch);
	delay_us(5000);
	i2c_lcd_send_char(ch);
	delay_us(5000);
	i2c_lcd_send_char(ch);
	delay_us(5000);
	i2c_lcd_send_char(ch);
	delay_us(5000);
	i2c_lcd_send_message(message,strlen(message));

	while(1)
	{

	}

	I2CDisable(I2C1);
	DisablePeriClk(I2C1);

	return 0;
}

void i2c_lcd_init(void)
{
	uint8_t control_message[2];

	control_message[0] = 0x00;  //Control Word

	//Initial Delay
	delay_us(200000);

	//1. Function Set (No. of Display Lines and Pixel Size Settings (2 lines, 5x8 pixels)
	control_message[1] = 0x28;
	I2CMasterSendData(I2C1,control_message,2, SlaveAddress);
	delay_us(50);

	//2. Display On/Off, Cursor On/Off and Cursor Blink Settings
	control_message[1] = 0x0F;
	I2CMasterSendData(I2C1,control_message,2, SlaveAddress);
	delay_us(50);

	//3. Display Clear
	control_message[1] = 0x01;
	I2CMasterSendData(I2C1,control_message,2, SlaveAddress);
	delay_us(3000);

	//4. Return Home
	control_message[1] = 0x02;
	I2CMasterSendData(I2C1,control_message,2, SlaveAddress);
	delay_us(3000);

	//5. Entry Mode Settings
	control_message[1] = 0x06;
	I2CMasterSendData(I2C1,control_message,2, SlaveAddress);
	delay_us(50);
}

void i2c_lcd_send_char(uint8_t c)
{
	uint8_t data_message[2];

	data_message[0] = 0x40;
	data_message[1] = c;

	I2CMasterSendData(I2C1,data_message,2, SlaveAddress);
	delay_us(50);
}

void i2c_lcd_send_message(char *message,uint8_t length)
{
	int i;

	for(i=0; i<length; i++)
	{
		i2c_lcd_send_char((uint8_t)message[i]);
	}
}

void config_system_clock(void)
{
	uint32_t *pRCC_CR = (uint32_t *) RCC_CR_REG_ADDR;
	uint32_t *pRCC_CFGR = (uint32_t *) RCC_CFGR_REG_ADDR;
	uint32_t *pRCC_PLLCFGR = (uint32_t *) RCC_PLLCFGR_REG_ADDR;
	uint32_t *pFLASH_ACR = (uint32_t *) FLASH_ACR_REG_ADDR;
	uint8_t PLL_SOURCE = PLL_SRC_HSI;
	uint16_t PLL_M_VALUE = 8;
	uint16_t PLL_N_VALUE = 80;
	uint16_t PLL_P_VALUE = 2;
	uint16_t PLL_Q_VALUE = 4;
	uint8_t SYSCLK_SRC = SYSCLK_SW_PLL;

	uint8_t PPRE1_VALUE = 4;
	uint8_t PPRE2_VALUE = 0;

	//Turning on the HSE Oscillator as the PLL Source
	*pRCC_CR |= (1 << HSE_ON_BITPOS);					//Turning on the HSE Oscillator
	while(!(*pRCC_CR & (1 << HSE_CHECK_BITPOS)));		//Checking whether HSE Oscillator is turned on and is stable

	//Configuring the PLL Oscillator
	// 1. Configuring the PLL Configuration Register (RCC_PLLCFGR)
	// 1a. Configuring 'M' Value
	*pRCC_PLLCFGR &= ~(63 << PLL_M_BITPOS);
	*pRCC_PLLCFGR |= (PLL_M_VALUE << PLL_M_BITPOS);

	// 1b. Configuring 'N' Value
	*pRCC_PLLCFGR &= ~(511 << PLL_N_BITPOS);
	*pRCC_PLLCFGR |= (PLL_N_VALUE << PLL_N_BITPOS);

	// 1c. Configuring 'P' Value
	*pRCC_PLLCFGR &= ~(3 << PLL_P_BITPOS);
	*pRCC_PLLCFGR |= (PLL_P_VALUE << PLL_P_BITPOS);

	// 1d. Configuring 'Q' Value
	*pRCC_PLLCFGR &= ~(15 << PLL_Q_BITPOS);
	*pRCC_PLLCFGR |= (PLL_Q_VALUE << PLL_Q_BITPOS);

	// 1.e Configuring the PLL Source (HSI or HSE)
	*pRCC_PLLCFGR &= ~(1 << PLL_SRC_BITPOS);
	*pRCC_PLLCFGR |= (PLL_SOURCE << PLL_SRC_BITPOS);

	// 2. Turning on the PLL Oscillator
	*pRCC_CR |= (1 << PLL_ON_BITPOS);					//Turning on the PLL Oscillator
	while(!(*pRCC_CR & (1 << PLL_CHECK_BITPOS)));		//Checking whether PLL Oscillator is turned on and is stable

	//Changing the FLASH Latency
	*pFLASH_ACR |= 0x3;						//Changing the FLASH Latency
	while(!(*pFLASH_ACR & 0x3));

	// Changing the System Clock (SYSCLK)
	//Changing the APB1 and APB2 Pre-Scalers
	*pRCC_CFGR |= (PPRE1_VALUE << RCC_CFGR_PPRE1_BITPOS);
	*pRCC_CFGR |= (PPRE2_VALUE << RCC_CFGR_PPRE2_BITPOS);
	*pRCC_CFGR &= ~(0x3 << SYSCLK_SW_BITPOS);
	*pRCC_CFGR |= (SYSCLK_SRC << SYSCLK_SW_BITPOS);
	while(!(*(pRCC_CFGR) & (0x2 << 2)));
}
