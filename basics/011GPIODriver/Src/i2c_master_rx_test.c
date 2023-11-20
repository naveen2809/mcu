/*
 * i2c_tx_test.c
 *
 *  Created on: 02-Aug-2023
 *      Author: naveen
 */



#include "stm32f4xx.h"
#include "i2c_driver.h"
#include "usart_driver.h"
#include "gpio_driver.h"
#include <stdint.h>
#include <string.h>

#define I2C_CONFIG_PERI_FREQ			0x10
#define I2C_CONFIG_CCR_VALUE			0x50
#define TRISE_VALUE						0x11
#define I2C1_SDA						GPIO_PIN_7
#define I2C1_SCL						GPIO_PIN_6
#define UART_TX_PIN						GPIO_PIN_2
#define UART_RX_PIN						GPIO_PIN_3
#define GPIO_PIN_NUM_INPUT				GPIO_PIN_0
#define GPIO_PIN_NUM_OUTPUT_1			GPIO_PIN_14
#define GPIO_PIN_NUM_OUTPUT_2			GPIO_PIN_15
#define GPIO_PIN_NUM_OUTPUT_3			GPIO_PIN_13
#define SLEEP_COUNT						500000

void NumToString(uint8_t Num, char *NumString);

struct USART_Handle_t Test_USART;
char message[128];
//char message[] = "Hello World from Discovery Board!!!\r\n";
//char message2[128];
uint8_t MessageLen[2];

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{
	struct I2C_Config_t I2C_Config;

	uint8_t SlaveAddress = 0x04;
	char MessageLenString[6] = {48,49,50,'\r','\n'};
	char MessageLenStringPrefix[] = "Message Len: ";
	//MessageLen[0]=strlen(message);

	memset(&I2C_Config,0,sizeof(I2C_Config));

	//GPIO Pin Configuration for Button and LED
	EnablePeriClk(GPIOA);
	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOA,GPIO_PIN_NUM_INPUT,GPIO_MODE_INPUT);
	GPIOSetMode(GPIOD,GPIO_PIN_NUM_OUTPUT_1,GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD,GPIO_PIN_NUM_OUTPUT_2,GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD,GPIO_PIN_NUM_OUTPUT_3,GPIO_MODE_OUTPUT);

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

	//GPIO Pin Configuration for USART1
	GPIOSetMode(GPIOA,UART_TX_PIN,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOA,UART_RX_PIN,GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,UART_TX_PIN,GPIO_ALTFN_7);
	GPIOSetAltFn(GPIOA,UART_RX_PIN,GPIO_ALTFN_7);
	GPIOSetOutputType(GPIOA,UART_TX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputType(GPIOA,UART_RX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputSpeed(GPIOA,UART_TX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetOutputSpeed(GPIOA,UART_RX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(GPIOA,UART_TX_PIN,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(GPIOA,UART_RX_PIN,GPIO_PULL_UP);

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

	//USART Port Configuration
	Test_USART.pUSART = (struct USART_RegDef_t *) USART2;
	Test_USART.USART_Config.USART_Mode = USART_MODE_TX_RX;
	Test_USART.USART_Config.USART_DataLength = USART_DATA_LEN_8_BITS;
	Test_USART.USART_Config.USART_StopBits = USART_STOP_BITS_1;
	Test_USART.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	Test_USART.USART_Config.USART_BaudRate = USART_SB_RATE_9600;
	Test_USART.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CNTRL_NONE;

	EnablePeriClk(USART2);
	USART_Init(&Test_USART);
	USART_PeripheralEnable(&Test_USART);

	while(1)
	{
		if(GPIOReadPin(GPIOA,GPIO_PIN_NUM_INPUT) == GPIO_HIGH)
		{
			GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_3);
			I2CMasterReceiveData(I2C1,MessageLen,2,SlaveAddress);
			GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_1);
			I2CMasterReceiveData(I2C1,(uint8_t *)message,MessageLen[0], SlaveAddress);
			GPIOTogglePin(GPIOD,GPIO_PIN_NUM_OUTPUT_2);
			NumToString(MessageLen[0], MessageLenString);
			USART_SendData(&Test_USART, (uint8_t *)MessageLenStringPrefix, (uint32_t) strlen(MessageLenStringPrefix));
			USART_SendData(&Test_USART, (uint8_t *)MessageLenString, 5);
			USART_SendData(&Test_USART, (uint8_t *)message, (uint32_t) MessageLen[0]);
			delay();
		}
	}

	//I2C Peripheral Disable
	I2CDisable(I2C1);
	DisablePeriClk(I2C1);

	//USART Peripheral Disable
	USART_PeripheralDisable(&Test_USART);
	DisablePeriClk(USART2);

	return 0;
}

void NumToString(uint8_t Num, char *NumString)
{
	char temp;

	if(Num > 99)
	{
		temp = Num/100;
		Num = Num - temp * 100;
	}
	else
	{
		temp = 0;
	}

	*NumString = temp + 48;
	NumString++;

	if(Num > 9)
	{
			temp = Num/10;
			Num = Num - temp * 10;

	}
	else
	{
		temp = 0;
	}

	*NumString = temp + 48;
	NumString++;

	*NumString = Num + 48;
	NumString++;

	*NumString = '\r';
	NumString++;

	*NumString = '\n';

}
