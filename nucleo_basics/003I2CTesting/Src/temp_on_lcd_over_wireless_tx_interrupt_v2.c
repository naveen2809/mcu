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

#define TX_QUEUE_SIZE	200
#define TRUE			1
#define FALSE			0
#define SLEEP_COUNT		50000

extern char cmd_buffer[];
extern uint8_t cmd_packet_length;

static uint8_t transmit_queue[TX_QUEUE_SIZE];

static uint32_t transmit_count = 0;
static uint8_t transmit_begin;

static void disable_interrupts(void);
static void enable_interrupts(void);
static void nrf_radio_enable_irq(void);

int main(void)
{
	uint8_t i;

	//NRF24L01 Radio Related Variables
	struct NRF_RADIO_CONFIG_t radio_config;
	uint8_t addr[] = {0xA1,0xA2,0xA3,0xA4,0xA5};      //nRF Radio Link Address

	disable_interrupts();

	//Configure the Timer
	configure_delay_timer();

	//Configure the UART Peripheral
	configure_uart();

	//Configure the SPI Port connecting to NRF24L01 Radio
	configure_spi();

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

	nrf_radio_enable_irq();

	//Prepare the Transmit Queue
	for(i=0;i<TX_QUEUE_SIZE;i++)
	{
		transmit_queue[i] = i+1;
	}

	printf("Beginning Transmission\r\n");

	delay_us(1000000);

	//Start the Packet Transmission
	transmit_begin = TRUE;
	transmit_count = 1;
	NVIC_IRQSetPending(NRF_RADIO_IRQ_NUM);

	enable_interrupts();

	while(1)
	{

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

static void nrf_radio_enable_irq(void)
{
	//Configure and Enable the IRQ in EXTI Controller and NVIC
	configure_external_gpio_interrupt(RADIO_PORT,IRQ_PIN,EXTI_FALLING_TRIGGER,NRF_RADIO_IRQ_NUM);

	return;
}

void EXTI0_IRQHandler(void)
{
	uint8_t status, interrupt_source;
	uint8_t tbuf[4];

	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	//Clearing the Interrupt in EXTI
	*pEXTI_PR |= (1 << IRQ_PIN);	//Clearing the EXTI_PR Register

	disable_interrupts();

	if(transmit_begin == TRUE)					//Transmit the first packet unconditionally
	{
		printf("Transmitting the first packet\r\n");
		transmit_begin = FALSE;

		tbuf[0] = transmit_queue[transmit_count-1];
		tbuf[1] = transmit_queue[transmit_count-1];
		tbuf[2] = transmit_queue[transmit_count-1];
		tbuf[3] = transmit_queue[transmit_count-1];
		nrf_radio_transmit_packet_interrupt(tbuf,4,NRF_RADIO_ACK);
	}
	else
	{
		//Check the status of previous transmission
		interrupt_source = nrf_radio_get_interrupt_source();
		if(interrupt_source == NRF_RADIO_INT_SRC_TX_DS)
		{
			status = NRF_RADIO_INT_SRC_TX_DS_BITPOS;
			printf("Packet %ld was transmitted successfully\r\n",transmit_count);
			transmit_count++;

			//Clear the interrupt by writing to STATUS Register
			cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_STATUS;
			cmd_buffer[1] = status;
			cmd_packet_length = 2;
			nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

			delay_us(SLEEP_COUNT);
		}
		else if(interrupt_source == NRF_RADIO_INT_SRC_MAX_RT)
		{
			status = NRF_RADIO_INT_SRC_MAX_RT_BITPOS;
			printf("Packet %ld transmission failed. Retrying\r\n",transmit_count);

			//Clear the interrupt by writing to STATUS Register
			cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_STATUS;
			cmd_buffer[1] = status;
			cmd_packet_length = 2;
			nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

			nrf_radio_flush_tx();

			delay_us(SLEEP_COUNT);
		}

		//Transmit the Next Packet
		if(transmit_count <= TX_QUEUE_SIZE)
		{
			tbuf[0] = transmit_queue[transmit_count-1];
			tbuf[1] = transmit_queue[transmit_count-1];
			tbuf[2] = transmit_queue[transmit_count-1];
			tbuf[3] = transmit_queue[transmit_count-1];
			nrf_radio_transmit_packet_interrupt(tbuf,4,NRF_RADIO_ACK);
		}
	}

	enable_interrupts();

	return;
}