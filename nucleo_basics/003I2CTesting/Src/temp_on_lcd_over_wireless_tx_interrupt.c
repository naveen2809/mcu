/*
 * temp_on_lcd_over_wireless_tx.c
 * Transferring the RH and Temperature data from DHT11 Sensor over nRFL01 Wireless Link
 * Reference: dht11_sensor_driver.c
 *  Created on: 24-Dec-2023
 *      Author: naveen
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "dht11_driver.h"
#include "nRFL01_driver.h"
#include "common_utils.h"
#include "external_interrupt_config.h"

#define TIMER5_IRQ_NUM	50

extern char cmd_buffer[];
extern uint8_t cmd_packet_length;


static volatile uint32_t transmit_count = 0;
static volatile uint8_t transmit_status;

static void disable_interrupts(void);
static void enable_interrupts(void);
static void configure_timer5(void);
static void nrf_radio_enable_irq(void);

static struct General_Purpose_Timer_Handle_t TIM5_Handle;

int main(void)
{
	//NRF24L01 Radio Related Variables
	struct NRF_RADIO_CONFIG_t radio_config;
	uint8_t addr[] = {0xA1,0xA2,0xA3,0xA4,0xA5};      //nRF Radio Link Address

	//Configure the Timer
	configure_delay_timer();

	//Configure the UART Peripheral
	configure_uart();

	//Configure the SPI Port connecting to NRF24L01 Radio
	configure_spi();

	//Disable printf() buffering
	setbuf(stdout, NULL);

	//Configure the NRF24L01 Radio
	memset(&radio_config,0,sizeof(radio_config));
	radio_config.radio_mode = NRF_RADIO_MODE_TX;
	radio_config.radio_channel = 90;
	radio_config.radio_data_rate = NRF_RADIO_DATA_RATE_250K;
	radio_config.radio_pa_level = NRF_RADIO_PA_LEVEL_3;
	radio_config.radio_enable_data_pipes = (NRF_RADIO_ERX_P0);
	radio_config.radio_enable_auto_acknowledgements = (NRF_RADIO_ENAA_P0);
	radio_config.radio_address_width = NRF_RADIO_ADDRESS_WIDTH_5B;
	radio_config.radio_auto_retransmit_count = NRF_RADIO_ARC_3;
	radio_config.radio_auto_retransmit_delay = NRF_RADIO_ARD_1000US;
	memcpy(&radio_config.radio_addr_p0, addr, sizeof(addr));
	memcpy(&radio_config.radio_addr_peer, addr, sizeof(addr));
	radio_config.radio_dynamic_payload_length_feature = NRF_RADIO_DPL_FEATURE_ENABLE;
	radio_config.radio_dynamic_payload_enable_pipes = NRF_RADIO_DPL_P0;
	radio_config.radio_ack_payload_feature = NRF_RADIO_ACK_PAYLOAD_FEATURE_DISABLE;
	radio_config.radio_dynamic_ack_feature = NRF_RADIO_DYNAMIC_ACK_FEATURE_ENABLE;

	configure_nrf_radio(&radio_config);

	configure_timer5();

	nrf_radio_enable_irq();

	printf("Beginning Transmission\r\n");

	while(1)
	{
		//printf("In Main While Loop!\r\n");
	}

	return 0;
}

static void disable_interrupts(void)
{
	__asm volatile ("CPSID I");

	return;
}

static void enable_interrupts(void)
{
	__asm volatile ("CPSIE I");

	return;
}

static void configure_timer5(void)
{
	TIM5_Handle.pGeneral_Purpose_Timer = (struct General_Purpose_Timer_RegDef_t *) TIM5;
	memset(&TIM5_Handle.General_Purpose_Timer_Config,0,sizeof(TIM5_Handle.General_Purpose_Timer_Config));
	TIM5_Handle.General_Purpose_Timer_Config.Timer_PreScalerValue = 0x0;
	TIM5_Handle.General_Purpose_Timer_Config.Timer_AutoReloadValue = 0x4C4B400;
	TIM5_Handle.General_Purpose_Timer_Config.Timer_UpdateInterruptConfig = TIMER_UPDATE_INTERRUPT_ENABLE;

	EnablePeriClk(TIM5);
	General_Purpose_Timer_Init(&TIM5_Handle);
	General_Purpose_Timer_PeripheralEnable(&TIM5_Handle);

	NVIC_EnableIRQ(TIMER5_IRQ_NUM);
}

void TIM5_IRQHandler(void)
{
	//DHT11 Sensor Related Variables
	uint8_t dht11_sensor_response;
	uint8_t dht11_sensor_data[5];

	disable_interrupts();

	//Check the status of previous packet transmission
	if(transmit_count > 0)
	{
		if(transmit_status == NRF_RADIO_PKT_TX_COMPLETE)
		{
			//Print Message
			printf("Packet %ld Transmitted Successfully\r\n",transmit_count);
		}
		else if(transmit_status == NRF_RADIO_PKT_TX_FAIL)
		{
			//Print Message
			printf("Packet %ld Transmission Failed\r\n",transmit_count);
		}
	}

	//Read the DHT11 Sensor
	dht11_sensor_response = read_dht11_sensor(dht11_sensor_data);

	if(dht11_sensor_response)
	{
		//Ensure that printf() is redirected to UART in "syscalls.c" file
		printf("DHT11 Sensor Read Successfully\r\n");
		printf("Relative Humidity: %d.%d\r\n",dht11_sensor_data[0],dht11_sensor_data[1]);
		printf("Temperature: %d.%d C\r\n",dht11_sensor_data[2],dht11_sensor_data[3]);

		//Transmit the sensor readings over wireless link
		nrf_radio_transmit_packet_interrupt(dht11_sensor_data,4,NRF_RADIO_ACK);
	}
	else
	{
		printf("DHT11 Sensor Reading Failed\r\n");
	}

	//Clear the TIM5 Interrupt
	General_Purpose_Timer_ClearUpdateEventFlag(&TIM5_Handle);

	transmit_count++;

	enable_interrupts();

	return;
}

static void nrf_radio_enable_irq(void)
{
	//Configure and Enable the IRQ in EXTI Controller and NVIC
	configure_external_gpio_interrupt(RADIO_PORT,IRQ_PIN,EXTI_FALLING_TRIGGER,NRF_RADIO_IRQ_NUM);

	return;
}

void EXTI0_IRQHandler(void)
{
	volatile uint8_t status;
	volatile uint8_t interrupt_source;

	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	disable_interrupts();

	//Clear the EXTI Interrupt
	*pEXTI_PR |= (1 << IRQ_PIN);	//Clearing the EXTI_PR Register

	interrupt_source = nrf_radio_get_interrupt_source();

	//printf("Interrupt Source: %d\r\n",interrupt_source);

	//Handle the interrupt source
	if(interrupt_source == NRF_RADIO_INT_SRC_TX_DS)
	{
		transmit_status = NRF_RADIO_PKT_TX_COMPLETE;

		status = NRF_RADIO_INT_SRC_TX_DS_BITPOS;

		//printf("Status Before Clearing = %d\r\n",nrf_radio_get_status_register());

		//Clear the interrupt by writing to STATUS Register
		cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_STATUS;
		cmd_buffer[1] = status;
		cmd_packet_length = 2;
		nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

		delay_us(5000);
	}
	else if(interrupt_source == NRF_RADIO_INT_SRC_MAX_RT)
	{
		transmit_status = NRF_RADIO_PKT_TX_FAIL;

		status = NRF_RADIO_INT_SRC_MAX_RT_BITPOS;

		//printf("Status Before Clearing = %d\r\n",nrf_radio_get_status_register());

		//Clear the interrupt by writing to STATUS Register
		cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_STATUS;
		cmd_buffer[1] = status;
		cmd_packet_length = 2;
		nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

		delay_us(5000);
	}

	enable_interrupts();

	return;
}

