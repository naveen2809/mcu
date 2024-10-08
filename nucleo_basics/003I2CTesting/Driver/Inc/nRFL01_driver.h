/*
 * nRFL01_driver.h
 *
 *  Created on: 24-Dec-2023
 *      Author: naveen
 */

#ifndef INC_NRFL01_DRIVER_H_
#define INC_NRFL01_DRIVER_H_

#include "stm32f4xx.h"
#include "gpio_driver.h"

#define RADIO_PORT			GPIOA
#define CE_PIN				2
#define IRQ_PIN				0
#define SS_PIN				4
#define NRF_RADIO_IRQ_NUM	6

#define NRF_RADIO_MODE_TX				0
#define NRF_RADIO_MODE_RX				1
#define NRF_RADIO_DATA_RATE_250K		0
#define NRF_RADIO_DATA_RATE_1M			1
#define NRF_RADIO_DATA_RATE_2M			2
#define NRF_RADIO_TX_DS_IRQ_ENABLE 		0
#define NRF_RADIO_TX_DS_IRQ_DISABLE 	1
#define NRF_RADIO_RX_DR_IRQ_ENABLE 		0
#define NRF_RADIO_RX_DR_IRQ_DISABLE 	1
#define NRF_RADIO_ACK					0
#define NRF_RADIO_NO_ACK				1
#define NRF_RADIO_PKT_TX_COMPLETE		0
#define NRF_RADIO_PKT_TX_FAIL			1
#define NRF_RADIO_PKT_TX_START			2
#define NRF_RADIO_INT_SRC_TX_DS			0
#define NRF_RADIO_INT_SRC_RX_DR			1
#define NRF_RADIO_INT_SRC_MAX_RT		2
#define NRF_RADIO_INT_SRC_NONE			3
#define NRF_RADIO_PA_LEVEL_0			0
#define NRF_RADIO_PA_LEVEL_1			1
#define NRF_RADIO_PA_LEVEL_2			2
#define NRF_RADIO_PA_LEVEL_3			3
#define NRF_RADIO_MAX_PKT_LENGTH		32
#define NRF_RADIO_MAX_ADDR_WIDTH		5

#define NRF_RADIO_RF_PWR_POS			1
#define NRF_RADIO_RF_DR_LOW_POS			5
#define NRF_RADIO_RF_DR_HIGH_POS		3

#define NRF_RADIO_ERX_P0				0x1
#define NRF_RADIO_ERX_P1				0x2
#define NRF_RADIO_ERX_P2				0x4
#define NRF_RADIO_ERX_P3				0x8
#define NRF_RADIO_ERX_P4				0x10
#define NRF_RADIO_ERX_P5				0x20

#define NRF_RADIO_ENAA_P0				0x1
#define NRF_RADIO_ENAA_P1				0x2
#define NRF_RADIO_ENAA_P2				0x4
#define NRF_RADIO_ENAA_P3				0x8
#define NRF_RADIO_ENAA_P4				0x10
#define NRF_RADIO_ENAA_P5				0x20

#define NRF_RADIO_ADDRESS_WIDTH_3B		0x01
#define NRF_RADIO_ADDRESS_WIDTH_4B		0x02
#define NRF_RADIO_ADDRESS_WIDTH_5B		0x03

#define NRF_RADIO_ARC_1					0x01
#define NRF_RADIO_ARC_2					0x02
#define NRF_RADIO_ARC_3					0x03
#define NRF_RADIO_ARC_4					0x04
#define NRF_RADIO_ARC_5					0x05
#define NRF_RADIO_ARC_6					0x06
#define NRF_RADIO_ARC_7					0x07
#define NRF_RADIO_ARC_8					0x08
#define NRF_RADIO_ARC_9					0x09
#define NRF_RADIO_ARC_10				0x0A
#define NRF_RADIO_ARC_11				0x0B
#define NRF_RADIO_ARC_12				0x0C
#define NRF_RADIO_ARC_13				0x0D
#define NRF_RADIO_ARC_14				0x0E
#define NRF_RADIO_ARC_15				0x0F

#define NRF_RADIO_ARD_250US				0x00
#define NRF_RADIO_ARD_500US				0x10
#define NRF_RADIO_ARD_750US				0x20
#define NRF_RADIO_ARD_1000US			0x30
#define NRF_RADIO_ARD_1250US			0x40
#define NRF_RADIO_ARD_1500US			0x50
#define NRF_RADIO_ARD_1750US			0x60
#define NRF_RADIO_ARD_2000US			0x70
#define NRF_RADIO_ARD_2250US			0x80
#define NRF_RADIO_ARD_2500US			0x90
#define NRF_RADIO_ARD_2750US			0xA0
#define NRF_RADIO_ARD_3000US			0xB0
#define NRF_RADIO_ARD_3250US			0xC0
#define NRF_RADIO_ARD_3500US			0xD0
#define NRF_RADIO_ARD_3750US			0xE0
#define NRF_RADIO_ARD_4000US			0xF0

#define NRF_RADIO_DPL_P0				0x01
#define NRF_RADIO_DPL_P1				0x02
#define NRF_RADIO_DPL_P2				0x04
#define NRF_RADIO_DPL_P3				0x08
#define NRF_RADIO_DPL_P4				0x10
#define NRF_RADIO_DPL_P5				0x20

#define NRF_RADIO_DPL_FEATURE_ENABLE			0x1
#define NRF_RADIO_DPL_FEATURE_DISABLE			0x0
#define NRF_RADIO_ACK_PAYLOAD_FEATURE_ENABLE	0x1
#define NRF_RADIO_ACK_PAYLOAD_FEATURE_DISABLE	0x0
#define NRF_RADIO_DYNAMIC_ACK_FEATURE_ENABLE	0x1
#define NRF_RADIO_DYNAMIC_ACK_FEATURE_DISABLE	0x0

#define NRF_RADIO_DPL_FEATURE_BITPOS			0x2
#define NRF_RADIO_ACK_PAYLOAD_FEATURE_BITPOS	0x1
#define NRF_RADIO_DYNAMIC_ACK_FEATURE_BITPOS	0x0
#define NRF_RADIO_INT_SRC_TX_DS_BITPOS			(1 << 5)
#define NRF_RADIO_INT_SRC_RX_DR_BITPOS			(1 << 6)
#define NRF_RADIO_INT_SRC_MAX_RT_BITPOS			(1 << 4)

#define NRF_RADIO_CMD_R_REGISTER_CONFIG				0x00
#define NRF_RADIO_CMD_R_REGISTER_EN_AA				0x01
#define NRF_RADIO_CMD_R_REGISTER_EN_RXADDR			0x02
#define NRF_RADIO_CMD_R_REGISTER_SETUP_AW			0x03
#define NRF_RADIO_CMD_R_REGISTER_SETUP_RETR			0x04
#define NRF_RADIO_CMD_R_REGISTER_RF_CH				0x05
#define NRF_RADIO_CMD_R_REGISTER_RF_SETUP			0x06
#define NRF_RADIO_CMD_R_REGISTER_STATUS				0x07
#define NRF_RADIO_CMD_R_REGISTER_OBSERVE_TX			0x08
#define NRF_RADIO_CMD_R_REGISTER_RPD				0x09
#define NRF_RADIO_CMD_R_REGISTER_RX_ADDR_P0			0x0A
#define NRF_RADIO_CMD_R_REGISTER_RX_ADDR_P1			0x0B
#define NRF_RADIO_CMD_R_REGISTER_RX_ADDR_P2			0x0C
#define NRF_RADIO_CMD_R_REGISTER_RX_ADDR_P3			0x0D
#define NRF_RADIO_CMD_R_REGISTER_RX_ADDR_P4			0x0E
#define NRF_RADIO_CMD_R_REGISTER_RX_ADDR_P5			0x0F
#define NRF_RADIO_CMD_R_REGISTER_TX_ADDR			0x10
#define NRF_RADIO_CMD_R_REGISTER_RX_PW_P0			0x11
#define NRF_RADIO_CMD_R_REGISTER_RX_PW_P1			0x12
#define NRF_RADIO_CMD_R_REGISTER_RX_PW_P2			0x13
#define NRF_RADIO_CMD_R_REGISTER_RX_PW_P3			0x14
#define NRF_RADIO_CMD_R_REGISTER_RX_PW_P4			0x15
#define NRF_RADIO_CMD_R_REGISTER_RX_PW_P5			0x16
#define NRF_RADIO_CMD_R_REGISTER_FIFO_STATUS		0x17
#define NRF_RADIO_CMD_R_REGISTER_ACK_PLD
#define NRF_RADIO_CMD_R_REGISTER_TX_PLD
#define NRF_RADIO_CMD_R_REGISTER_RX_PLD
#define NRF_RADIO_CMD_R_REGISTER_DYNPD				0x1C
#define NRF_RADIO_CMD_R_REGISTER_FEATURE			0x1D

#define NRF_RADIO_CMD_W_REGISTER_CONFIG				0x20
#define NRF_RADIO_CMD_W_REGISTER_EN_AA				0x21
#define NRF_RADIO_CMD_W_REGISTER_EN_RXADDR			0x22
#define NRF_RADIO_CMD_W_REGISTER_SETUP_AW			0x23
#define NRF_RADIO_CMD_W_REGISTER_SETUP_RETR			0x24
#define NRF_RADIO_CMD_W_REGISTER_RF_CH				0x25
#define NRF_RADIO_CMD_W_REGISTER_RF_SETUP			0x26
#define NRF_RADIO_CMD_W_REGISTER_STATUS				0x27
#define NRF_RADIO_CMD_W_REGISTER_OBSERVE_TX			0x28
#define NRF_RADIO_CMD_W_REGISTER_RPD				0x29
#define NRF_RADIO_CMD_W_REGISTER_RX_ADDR_P0			0x2A
#define NRF_RADIO_CMD_W_REGISTER_RX_ADDR_P1			0x2B
#define NRF_RADIO_CMD_W_REGISTER_RX_ADDR_P2			0x2C
#define NRF_RADIO_CMD_W_REGISTER_RX_ADDR_P3			0x2D
#define NRF_RADIO_CMD_W_REGISTER_RX_ADDR_P4			0x2E
#define NRF_RADIO_CMD_W_REGISTER_RX_ADDR_P5			0x2F
#define NRF_RADIO_CMD_W_REGISTER_TX_ADDR			0x30
#define NRF_RADIO_CMD_W_REGISTER_RX_PW_P0			0x31
#define NRF_RADIO_CMD_W_REGISTER_RX_PW_P1			0x32
#define NRF_RADIO_CMD_W_REGISTER_RX_PW_P2			0x33
#define NRF_RADIO_CMD_W_REGISTER_RX_PW_P3			0x34
#define NRF_RADIO_CMD_W_REGISTER_RX_PW_P4			0x35
#define NRF_RADIO_CMD_W_REGISTER_RX_PW_P5			0x36
#define NRF_RADIO_CMD_W_REGISTER_FIFO_STATUS		0x37
#define NRF_RADIO_CMD_W_REGISTER_ACK_PLD
#define NRF_RADIO_CMD_W_REGISTER_TX_PLD
#define NRF_RADIO_CMD_W_REGISTER_RX_PLD
#define NRF_RADIO_CMD_W_REGISTER_DYNPD				0x3C
#define NRF_RADIO_CMD_W_REGISTER_FEATURE			0x3D

#define NRF_RADIO_CMD_R_RX_PAYLOAD					0x61
#define NRF_RADIO_CMD_W_TX_PAYLOAD					0xA0
#define NRF_RADIO_CMD_FLUSH_TX						0xE1
#define NRF_RADIO_CMD_FLUSH_RX						0xE2
#define NRF_RADIO_CMD_REUSE_TX_PL					0xE3
#define NRF_RADIO_CMD_R_RX_PL_WID					0x60
#define NRF_RADIO_CMD_W_ACK_PAYLOAD_PIPE0			0xA8
#define NRF_RADIO_CMD_W_ACK_PAYLOAD_PIPE1			0xA9
#define NRF_RADIO_CMD_W_ACK_PAYLOAD_PIPE2			0xAA
#define NRF_RADIO_CMD_W_ACK_PAYLOAD_PIPE3			0xAB
#define NRF_RADIO_CMD_W_ACK_PAYLOAD_PIPE4			0xAC
#define NRF_RADIO_CMD_W_ACK_PAYLOAD_PIPE5			0xAD
#define NRF_RADIO_CMD_W_TX_PAYLOAD_NOACK			0xB0
#define NRF_RADIO_CMD_NOP							0xFF

struct NRF_RADIO_CONFIG_t
{
	uint8_t radio_mode;
	uint8_t radio_channel;
	uint8_t radio_data_rate;
	uint8_t radio_pa_level;
	uint8_t radio_enable_data_pipes;
	uint8_t	radio_enable_auto_acknowledgements;
	uint8_t radio_address_width;
	uint8_t radio_auto_retransmit_count;
	uint8_t radio_auto_retransmit_delay;
	uint8_t radio_addr_p0[NRF_RADIO_MAX_ADDR_WIDTH];
	uint8_t radio_addr_p1[NRF_RADIO_MAX_ADDR_WIDTH];
	uint8_t radio_addr_p2[NRF_RADIO_MAX_ADDR_WIDTH];
	uint8_t radio_addr_p3[NRF_RADIO_MAX_ADDR_WIDTH];
	uint8_t radio_addr_p4[NRF_RADIO_MAX_ADDR_WIDTH];
	uint8_t radio_addr_p5[NRF_RADIO_MAX_ADDR_WIDTH];
	uint8_t radio_addr_peer[NRF_RADIO_MAX_ADDR_WIDTH];
	uint8_t radio_dynamic_payload_length_feature;
	uint8_t radio_dynamic_payload_enable_pipes;
	uint8_t radio_ack_payload_feature;
	uint8_t radio_dynamic_ack_feature;

};

void configure_nrf_radio(struct NRF_RADIO_CONFIG_t * radio_config);
uint8_t nrf_radio_transmit_packet(uint8_t *buffer,uint8_t length,uint8_t ack_policy);
uint8_t nrf_radio_transmit_packet_polling(uint8_t *buffer,uint8_t length,uint8_t ack_policy);
void nrf_radio_receive_packet_callback(uint8_t *buffer, uint8_t length);
void nrf_radio_packet_received_application_callback(uint8_t *buffer, uint8_t length);
uint8_t nrf_radio_get_status_register(void);
uint8_t nrf_radio_get_interrupt_source(void);
uint8_t nrf_radio_get_rx_packet_length(void);
void nrf_radio_send_packet_to_fifo(uint8_t *buffer,uint8_t length,uint8_t ack_policy);
void nrf_radio_retreive_packet_from_fifo(void);
void nrf_radio_cmd_write(uint8_t *buffer,uint8_t length);
void nrf_radio_cmd_read(uint8_t *buffer,uint8_t length);
void nrf_radio_flush_tx(void);
void nrf_radio_interrupt_callback(void);
void nrf_radio_rx_polling(void);
void nrf_radio_transmit_packet_interrupt(uint8_t *buffer,uint8_t length,uint8_t ack_policy);

#endif /* INC_NRFL01_DRIVER_H_ */
