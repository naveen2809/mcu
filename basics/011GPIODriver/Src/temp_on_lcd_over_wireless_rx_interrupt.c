/*
 * temp_on_lcd_over_wireless_rx.c
 * Displaying the RH and Temperature data received over nRFL01 Wireless Link
 * Reference: dht11_sensor_driver.c and lcd_pcf8574_test.c
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

extern char rx_buffer[NRF_RADIO_MAX_PKT_LENGTH];
extern char cmd_buffer[NRF_RADIO_MAX_PKT_LENGTH + 1];
extern uint8_t rx_packet_length;
extern uint8_t cmd_packet_length;
extern uint8_t nrf_packet_tx_state;

extern struct USART_Handle_t Test_USART;

int main(void)
{
	//NRF24L01 Radio Related Variables
	struct NRF_RADIO_CONFIG_t radio_config;
	uint8_t addr[] = {0xA1,0xA2,0xA3,0xA4,0xA5};      //nRF Radio Link Address

	char message[] = "In Main While Loop!\r\n";

	//Configure the Timer
	configure_delay_timer();

	//Configure UART
	configure_uart();

	//Configure the LCD Display
	//a. Configure the I2C Peripheral
	configure_i2c();
	//configure_uart();

	//b. Disable printf() buffering
	setbuf(stdout, NULL);

	//c. Initialize the LCD Display
	delay_us(1000000);
	lcd_pcf8574_init();
	delay_us(100000);

	//Configure the SPI Port connecting to NRF24L01 Radio
	configure_spi();

	//Configure the NRF24L01 Radio
	memset(&radio_config,0,sizeof(radio_config));
	radio_config.radio_mode = NRF_RADIO_MODE_RX;
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

	while(1)
	{
		USART_SendData(&Test_USART, (uint8_t *)message, strlen(message));
	}

	return 0;
}

void nrf_radio_packet_received_application_callback(uint8_t *buffer, uint8_t length)
{
	//Ensure that printf is redirected to LCD is "syscalls.c" file

	//Print RH Data
	lcd_pcf8574_clear_screen();
	delay_us(3000);
	lcd_pcf8574_return_home();
	delay_us(3000);
	printf("RH:   %d.%d",buffer[0],buffer[1]);

	//Print TEMP Data
	lcd_pcf8574_set_position(1,0);
	delay_us(3000);
	printf("TEMP: %d.%d C",buffer[2],buffer[3]);

	return;
}

void nrf_radio_enable_irq(void)
{
	//Configure and Enable the IRQ in EXTI Controller and NVIC
	configure_external_gpio_interrupt(RADIO_PORT,IRQ_PIN,EXTI_FALLING_TRIGGER,NRF_RADIO_IRQ_NUM);

	return;
}

void EXTI0_IRQHandler(void)
{
	uint8_t status, interrupt_source;

	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	interrupt_source = nrf_radio_get_interrupt_source();

	//printf("Interrupt Source: %d\r\n",interrupt_source);

	//Handle the interrupt source
	if(interrupt_source == NRF_RADIO_INT_SRC_RX_DR)
	{
		nrf_radio_retreive_packet_from_fifo();
		nrf_radio_packet_received_application_callback((uint8_t *)(rx_buffer+1),(rx_packet_length-1));
		status = NRF_RADIO_INT_SRC_RX_DR_BITPOS;

		//printf("Status Before Clearing = %d\r\n",nrf_radio_get_status_register());

		//4. Clear the interrupt by writing to STATUS Register
		cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_STATUS;
		cmd_buffer[1] = status;
		cmd_packet_length = 2;
		nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

		delay_us(5000);
	}

	//Clearing the interrupt in EXTI
	*pEXTI_PR |= (1 << IRQ_PIN);


	return;
}