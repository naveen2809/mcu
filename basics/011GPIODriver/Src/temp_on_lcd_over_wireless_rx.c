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

int main(void)
{
	//NRF24L01 Radio Related Variables
	struct NRF_RADIO_CONFIG_t radio_config;
	uint8_t addr[] = {0xA1,0xA2,0xA3,0xA4,0xA5};      //nRF Radio Link Address

	//Configure the Timer
	configure_delay_timer();

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

	while(1)
	{
		nrf_radio_rx_polling();
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
