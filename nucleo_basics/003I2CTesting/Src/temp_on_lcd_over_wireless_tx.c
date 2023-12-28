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

int main(void)
{
	uint32_t count = 0;

	//DHT11 Sensor Related Variables
	uint8_t dht11_sensor_response;
	uint8_t dht11_sensor_data[5];

	//NRF24L01 Radio Related Variables
	struct NRF_RADIO_CONFIG_t radio_config;
	uint8_t status;
	uint8_t addr[] = {0xA1,0xA2,0xA3,0xA4,0xA5};      //nRF Radio Link Address

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

	while(1)
	{
		//Read the DHT11 Sensor
		dht11_sensor_response = read_dht11_sensor(dht11_sensor_data);

		if(dht11_sensor_response)
		{
			//Ensure that printf() is redirected to UART in "syscalls.c" file
			printf("DHT11 Sensor Read Successfully\r\n");
			printf("Relative Humidity: %2d.%2d\r\n",dht11_sensor_data[0],dht11_sensor_data[1]);
			printf("Temperature: %2d.%2d C\r\n",dht11_sensor_data[2],dht11_sensor_data[3]);

			//Transmit the sensor readings over wireless link
			status = nrf_radio_transmit_packet_polling(dht11_sensor_data,4,NRF_RADIO_NO_ACK);
			if(status == NRF_RADIO_PKT_TX_COMPLETE)
			{
				//Print Message
				printf("Packet %ld Transmitted Successfully\r\n",++count);
			}
			else if(status == NRF_RADIO_PKT_TX_FAIL)
			{
				//Print Message
				printf("Packet %ld Transmission Failed\r\n",++count);
			}
		}
		else
		{
			printf("DHT11 Sensor Reading Failed\r\n");
		}

		delay_us(5000000);
	}

/*

	while(1)
	{
		//Transmit the sensor readings over wireless link
		status = nrf_radio_transmit_packet_polling(dht11_sensor_data,4,NRF_RADIO_ACK);
		if(status == NRF_RADIO_PKT_TX_COMPLETE)
		{
			//Print Message
			printf("Packet %ld Transmitted Successfully\r\n",++count);
		}
		else if(status == NRF_RADIO_PKT_TX_FAIL)
		{
			//Print Message
			printf("Packet %ld Transmission Failed\r\n",++count);
		}
	}
*/
	return 0;
}
