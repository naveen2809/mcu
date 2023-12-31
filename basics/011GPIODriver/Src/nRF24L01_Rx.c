/*
 * nRF24L01_Rx.c
 *
 *  Created on: 18-Dec-2023
 *      Author: naveen
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "spi_driver.h"
#include "gpio_driver.h"
#include "usart_driver.h"
#include "common_utilities.h"
#include "external_interrupt_config.h"

#define SPI_DEV			SPI2
#define SPI_PORT		GPIOB
#define SPI_PIN_MOSI	15
#define SPI_PIN_MISO	14
#define SPI_PIN_CLK		13
#define SPI_PIN_SS		12							//Not connected. Doing slave select using GPIO (SS_PIN)

#define RADIO_PORT		GPIOA
#define CE_PIN			2
#define IRQ_PIN			0
#define SS_PIN			4

#define UART_GPIO_PORT		GPIOD
#define UART_TX_PIN			GPIO_PIN_5
#define UART_RX_PIN			GPIO_PIN_6

#define GPIO_LED_PORT		GPIOD
#define GPIO_LED_PIN		GPIO_PIN_12

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
#define	NRF_RADIO_INT_SRC_NONE			3
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
void nrf_radio_receive_packet_callback(uint8_t *buffer, uint8_t length);
uint8_t nrf_radio_get_status_register(void);
uint8_t nrf_radio_get_interrupt_source(void);
uint8_t nrf_radio_get_rx_packet_length(void);
void nrf_radio_send_packet_to_fifo(uint8_t *buffer,uint8_t length,uint8_t ack_policy);
void nrf_radio_retreive_packet_from_fifo(void);
void nrf_radio_cmd_write(uint8_t *buffer,uint8_t length);
void nrf_radio_cmd_read(uint8_t *buffer,uint8_t length);
void nrf_radio_rx_polling(void);

void configure_spi(void);
void configure_radio_interrupts(void);
void configure_uart(void);
void UART_SendChar(uint8_t ch);
void configure_gpio_led(void);
void gpio_led_turn_on(void);
void gpio_led_turn_off(void);

char tx_buffer[NRF_RADIO_MAX_PKT_LENGTH] = "Hello World\r\n";
char rx_buffer[NRF_RADIO_MAX_PKT_LENGTH];
char cmd_buffer[NRF_RADIO_MAX_PKT_LENGTH + 1];
uint8_t tx_packet_length;
uint8_t rx_packet_length;
uint8_t cmd_packet_length;
uint8_t nrf_packet_tx_state;

struct USART_Handle_t Test_USART;

int main(void)
{
	struct NRF_RADIO_CONFIG_t radio_config;

	uint8_t addr[] = {0xA1,0xA2,0xA3,0xA4,0xA5};

	tx_packet_length = strlen(tx_buffer);

	//1.a Configure the Timer Peripheral
	configure_delay_timer();

	//1.b Configure the UART Peripheral
	configure_uart();

	//1.c Configure Radio Interrupts
	//configure_radio_interrupts();

	//2. Configure the SPI Port connecting to NRF24L01 Radio
	configure_spi();

	configure_gpio_led();

	//3. Configure the NRF24L01 Radio
	memset(&radio_config,0,sizeof(radio_config));
	radio_config.radio_mode = NRF_RADIO_MODE_RX;
	radio_config.radio_channel = 10;
	radio_config.radio_data_rate = NRF_RADIO_DATA_RATE_250K;
	radio_config.radio_pa_level = NRF_RADIO_PA_LEVEL_0;
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

void configure_spi(void)
{
	struct SPI_Config_t SPI_Dev_Config;

	//Configuring the GPIO Pins for the SPI Port
	EnablePeriClk(SPI_PORT);
	GPIOSetMode(SPI_PORT,SPI_PIN_MOSI,GPIO_MODE_ALTFN);
	GPIOSetMode(SPI_PORT,SPI_PIN_MISO,GPIO_MODE_ALTFN);
	GPIOSetMode(SPI_PORT,SPI_PIN_CLK,GPIO_MODE_ALTFN);
	GPIOSetMode(SPI_PORT,SPI_PIN_SS,GPIO_MODE_ALTFN);
	GPIOSetAltFn(SPI_PORT,SPI_PIN_MOSI,GPIO_ALTFN_5);
	GPIOSetAltFn(SPI_PORT,SPI_PIN_MISO,GPIO_ALTFN_5);
	GPIOSetAltFn(SPI_PORT,SPI_PIN_CLK,GPIO_ALTFN_5);
	GPIOSetAltFn(SPI_PORT,SPI_PIN_SS,GPIO_ALTFN_5);

	//Configuring the SPI Peripheral
	SPI_Dev_Config.SPIDeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI_Dev_Config.SPIClockPol = SPI_CLK_POL_0;
	SPI_Dev_Config.SPIClockPhase = SPI_CLK_PHA_LE;
	SPI_Dev_Config.SPIClockFreq = SPI_CLK_FREQ_DIV4;
	SPI_Dev_Config.SPIDataFrameFormat = SPI_DFF_8_BITS;
	SPI_Dev_Config.SPISoftwareSlaveManagement = SPI_SW_SLAVE_MGNT_DI;
	SPI_Dev_Config.SPISSIFlag = SPI_SSI_1;
	SPI_Dev_Config.SPISSOEFlag = SPI_SSOE_EN;

	EnablePeriClk(SPI_DEV);
	SPIPeriConfig(SPI_DEV, &SPI_Dev_Config);
	SPIEnable(SPI_DEV);
}

void configure_nrf_radio(struct NRF_RADIO_CONFIG_t * radio_config)
{
	uint8_t i;

	//1. Configure the GPIO pins for CE and IRQ pins
	EnablePeriClk(RADIO_PORT);
	GPIOSetMode(RADIO_PORT,CE_PIN,GPIO_MODE_OUTPUT);
	GPIOWritePin(RADIO_PORT,CE_PIN,GPIO_LOW);
	GPIOSetMode(RADIO_PORT,IRQ_PIN,GPIO_MODE_INPUT);
	GPIOSetPullUpDownConfig(RADIO_PORT,IRQ_PIN,GPIO_PULL_UP);
	GPIOSetMode(RADIO_PORT,SS_PIN,GPIO_MODE_OUTPUT);
	GPIOWritePin(RADIO_PORT,SS_PIN,GPIO_HIGH);

	//2. Configure the Radio Mode

	//3. Configure the RF Channel, Data Rate and PA Level
	//3.1 Configure the RF Channel
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_RF_CH;
	cmd_buffer[1] = radio_config->radio_channel;
	cmd_packet_length = 2;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	//3.2 Configure the Data Rate and PA Level
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_RF_SETUP;

	cmd_buffer[1] &= 0x00;
	cmd_buffer[1] |= (radio_config->radio_pa_level << NRF_RADIO_RF_PWR_POS);

	if(radio_config->radio_data_rate == NRF_RADIO_DATA_RATE_250K)
	{
		cmd_buffer[1] |= (1 << NRF_RADIO_RF_DR_LOW_POS);
		cmd_buffer[1] |= (0 << NRF_RADIO_RF_DR_HIGH_POS);
	}
	else if(radio_config->radio_data_rate == NRF_RADIO_DATA_RATE_1M)
	{
		cmd_buffer[1] |= (0 << NRF_RADIO_RF_DR_LOW_POS);
		cmd_buffer[1] |= (0 << NRF_RADIO_RF_DR_HIGH_POS);
	}
	else if(radio_config->radio_data_rate == NRF_RADIO_DATA_RATE_2M)
	{
		cmd_buffer[1] |= (0 << NRF_RADIO_RF_DR_LOW_POS);
		cmd_buffer[1] |= (1 << NRF_RADIO_RF_DR_HIGH_POS);
	}

	cmd_packet_length = 2;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	//4. Configure Data Pipes
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_EN_RXADDR;
	cmd_buffer[1] = radio_config->radio_enable_data_pipes;
	cmd_packet_length = 2;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	//5. Configure Auto-Acknowledgements
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_EN_AA;
	cmd_buffer[1] = radio_config->radio_enable_auto_acknowledgements;
	cmd_packet_length = 2;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	//6. Configure Address Width
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_SETUP_AW;
	cmd_buffer[1] = radio_config->radio_address_width;
	cmd_packet_length = 2;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	//7. Configure Retransmission Parameters
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_SETUP_RETR;
	cmd_buffer[1] = (radio_config->radio_auto_retransmit_count | radio_config->radio_auto_retransmit_delay);
	cmd_packet_length = 2;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	//8. Configure Own Address (RX_ADDR_P0)
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_RX_ADDR_P0;
	for(i=1;i<=radio_config->radio_address_width;i++)
	{
		cmd_buffer[i] = radio_config->radio_addr_p0[i];
	}
	cmd_packet_length = radio_config->radio_address_width + 1;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	//9. Configure Peer Address (TX_ADDR)
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_TX_ADDR;
	for(i=1;i<=radio_config->radio_address_width;i++)
	{
		cmd_buffer[i] = radio_config->radio_addr_peer[i];
	}
	cmd_packet_length = radio_config->radio_address_width + 1;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	//10. Configure Dynamic Pay-load Length Feature for Individual Pipes
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_DYNPD;
	cmd_buffer[1] = radio_config->radio_dynamic_payload_enable_pipes;
	cmd_packet_length = 2;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	//11. Configure Feature Register
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_FEATURE;
	cmd_buffer[1] &= 0x00;
	cmd_buffer[1] |= (radio_config->radio_dynamic_payload_length_feature << NRF_RADIO_DPL_FEATURE_BITPOS);
	cmd_buffer[1] |= (radio_config->radio_ack_payload_feature << NRF_RADIO_ACK_PAYLOAD_FEATURE_BITPOS);
	cmd_buffer[1] |= (radio_config->radio_dynamic_ack_feature << NRF_RADIO_DYNAMIC_ACK_FEATURE_BITPOS);
	cmd_packet_length = 2;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	//12. Configure CONFIG Register
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_CONFIG;
	cmd_buffer[1] &= 0x00;
	if(radio_config->radio_mode == NRF_RADIO_MODE_TX)
	{
		cmd_buffer[1] = 0x0A;
	}
	else if(radio_config->radio_mode == NRF_RADIO_MODE_RX)
	{
		cmd_buffer[1] = 0x0B;
	}
	cmd_packet_length = 2;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	if(radio_config->radio_mode == NRF_RADIO_MODE_RX)
	{
		delay_us(50);
		GPIOWritePin(RADIO_PORT,CE_PIN,GPIO_HIGH);
	}

	return;
}

uint8_t nrf_radio_transmit_packet(uint8_t *buffer,uint8_t length,uint8_t ack_policy)
{

	nrf_packet_tx_state = NRF_RADIO_PKT_TX_START;

	//1. Send packet to radio using SPI interface
	nrf_radio_send_packet_to_fifo(buffer,length,ack_policy);

	//2. Pulse the PTX CE line for more than 10us
	GPIOWritePin(RADIO_PORT,CE_PIN,GPIO_HIGH);
	delay_us(10);
	GPIOWritePin(RADIO_PORT,CE_PIN,GPIO_LOW);

	//3. Wait for confirmation of packet transmission from NRF24L01 radio
	while(nrf_packet_tx_state == NRF_RADIO_PKT_TX_START);              //Packet TX state is updated in the ISR

	return nrf_packet_tx_state;
}

void nrf_radio_receive_packet_callback(uint8_t *buffer, uint8_t length)
{
	uint8_t i;

	//Print message
	printf("Packet Received from Peer Radio\r\n");

	//Print packet length
	//The actual length of the message is one less than the value of length
	printf("Packet Length: %d\r\n",length);

	//Print packet contents
	//The actual Rx packet contents are stored from buffer[1] onwards as buffer[0] contains the value of STATUS Register
	for(i=1;i<=length;i++)
	{
		printf("%c",buffer[i]);
	}

	if(buffer[1] == 'X' && buffer[2] == 'X' && buffer[3] == 'X' && buffer[4] == 'X')
	{
		gpio_led_turn_on();
	}

	if(buffer[1] == 'Y' && buffer[2] == 'Y' && buffer[3] == 'Y' && buffer[4] == 'Y')
	{
		gpio_led_turn_off();
	}

	return;
}

void nrf_radio_interrupt_callback(void)
{
	uint8_t interrupt_source, status;

	//1. Get the STATUS Register
	status = nrf_radio_get_status_register();

	//2. Identify interrupt source
	interrupt_source = nrf_radio_get_interrupt_source();

	//3. Handle the interrupt source
	if(interrupt_source == NRF_RADIO_INT_SRC_RX_DR)
	{
		nrf_radio_retreive_packet_from_fifo();
		nrf_radio_receive_packet_callback((uint8_t *)rx_buffer,rx_packet_length);
		status |= NRF_RADIO_INT_SRC_RX_DR_BITPOS;
	}
	else if(interrupt_source == NRF_RADIO_INT_SRC_TX_DS)
	{
		nrf_packet_tx_state = NRF_RADIO_PKT_TX_COMPLETE;
		status |= NRF_RADIO_INT_SRC_TX_DS_BITPOS;
	}
	else if(interrupt_source == NRF_RADIO_INT_SRC_MAX_RT)
	{
		nrf_packet_tx_state = NRF_RADIO_PKT_TX_FAIL;
		status |= NRF_RADIO_INT_SRC_MAX_RT_BITPOS;
	}

	//4. Clear the interrupt by writing to STATUS Register
	cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_STATUS;
	cmd_buffer[1] = status;
	cmd_packet_length = 2;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	return;
}

uint8_t nrf_radio_get_status_register(void)
{
	uint8_t status;

	//1. Read the STATUS register (using NOP command)
	cmd_buffer[0] = NRF_RADIO_CMD_NOP;
	cmd_packet_length = 1;

	GPIOWritePin(RADIO_PORT,SS_PIN,GPIO_LOW);
	SPIMasterRecvData(SPI_DEV,(uint8_t *)cmd_buffer,(uint8_t *)rx_buffer,cmd_packet_length);
	GPIOWritePin(RADIO_PORT,SS_PIN,GPIO_HIGH);

	status = (uint8_t) rx_buffer[0];

	return status;
}

uint8_t nrf_radio_get_interrupt_source(void)
{
	uint8_t status, interrupt_source;

	//1. Read the STATUS register
	status = nrf_radio_get_status_register();

	if(status & NRF_RADIO_INT_SRC_TX_DS_BITPOS)
	{
		interrupt_source = NRF_RADIO_INT_SRC_TX_DS;
	}
	else if(status & NRF_RADIO_INT_SRC_RX_DR_BITPOS)
	{
		interrupt_source = NRF_RADIO_INT_SRC_RX_DR;
	}
	else if(status & NRF_RADIO_INT_SRC_MAX_RT_BITPOS)
	{
		interrupt_source = NRF_RADIO_INT_SRC_MAX_RT;
	}
	else
	{
		interrupt_source = NRF_RADIO_INT_SRC_NONE;
	}

	return interrupt_source;
}

void nrf_radio_retreive_packet_from_fifo(void)
{
	uint8_t packet_length, i, status;

	//1. Get the STATUS Register
	status = nrf_radio_get_status_register();
	printf("Status: %d\r\n",status);

	//1. Get the length of the RX Pay-load (from Data Pipe P0)
	packet_length = nrf_radio_get_rx_packet_length();

	printf("Packet Length: %d\r\n",packet_length);

	//2. Read the RX Pay-load
	cmd_buffer[0] = NRF_RADIO_CMD_R_RX_PAYLOAD;
	for(i=1;i<=packet_length;i++)
		cmd_buffer[i] = 0x00;
	cmd_packet_length = packet_length + 1;

	GPIOWritePin(RADIO_PORT,SS_PIN,GPIO_LOW);
	SPIMasterRecvData(SPI_DEV,(uint8_t *)cmd_buffer,(uint8_t *)rx_buffer,cmd_packet_length);
	GPIOWritePin(RADIO_PORT,SS_PIN,GPIO_HIGH);

	//3. Update the global variable rx_packet_length
	rx_packet_length = packet_length;
}

uint8_t nrf_radio_get_rx_packet_length(void)
{

	uint8_t length;
	//1. Read the RX_PW_P0 Register
	cmd_buffer[0] = NRF_RADIO_CMD_R_RX_PL_WID;
	cmd_buffer[1] = 0x00;
	cmd_packet_length = 2;

	GPIOWritePin(RADIO_PORT,SS_PIN,GPIO_LOW);
	SPIMasterRecvData(SPI_DEV,(uint8_t *)cmd_buffer,(uint8_t *)rx_buffer,cmd_packet_length);
	GPIOWritePin(RADIO_PORT,SS_PIN,GPIO_HIGH);

	length = (uint8_t) rx_buffer[1];

	return length;
}

void nrf_radio_send_packet_to_fifo(uint8_t *buffer,uint8_t length,uint8_t ack_policy)
{
	uint8_t i;

	if(ack_policy == NRF_RADIO_ACK)
	{
		cmd_buffer[0] = NRF_RADIO_CMD_W_TX_PAYLOAD;
	}
	else if(ack_policy == NRF_RADIO_NO_ACK)
	{
		cmd_buffer[0] = NRF_RADIO_CMD_W_TX_PAYLOAD_NOACK;
	}

	for(i=1;i<=length;i++)
	{
		cmd_buffer[i] = buffer[i];
	}

	cmd_packet_length = length + 1;
	nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);

	return;
}

void nrf_radio_cmd_write(uint8_t *buffer,uint8_t length)
{
	GPIOWritePin(RADIO_PORT,SS_PIN,GPIO_LOW);
	SPISendData(SPI_DEV,buffer,length);
	GPIOWritePin(RADIO_PORT,SS_PIN,GPIO_HIGH);
}

void configure_uart(void)
{
	//GPIO Pin Configuration
	EnablePeriClk(UART_GPIO_PORT);
	GPIOSetMode(UART_GPIO_PORT,UART_TX_PIN,GPIO_MODE_ALTFN);
	GPIOSetMode(UART_GPIO_PORT,UART_RX_PIN,GPIO_MODE_ALTFN);
	GPIOSetAltFn(UART_GPIO_PORT,UART_TX_PIN,GPIO_ALTFN_7);
	GPIOSetAltFn(UART_GPIO_PORT,UART_RX_PIN,GPIO_ALTFN_7);
	GPIOSetOutputType(UART_GPIO_PORT,UART_TX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputType(UART_GPIO_PORT,UART_RX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputSpeed(UART_GPIO_PORT,UART_TX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetOutputSpeed(UART_GPIO_PORT,UART_RX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(UART_GPIO_PORT,UART_TX_PIN,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(UART_GPIO_PORT,UART_RX_PIN,GPIO_PULL_UP);

	//USART Configuration
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
}

void UART_SendChar(uint8_t ch)
{
	USART_SendData(&Test_USART, &ch, 1);
}

void configure_radio_interrupts(void)
{
	configure_external_interrupt();
}

void EXTI0_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	//1. Handling the interrupt
	nrf_radio_interrupt_callback();

	//2. Clearing the EXTI_PR Register
	*pEXTI_PR |= (1 << INTERRUPT_PIN);

	return;
}

void nrf_radio_rx_polling(void)
{

	uint8_t status, interrupt_source;

	while(1)
	{
		status = nrf_radio_get_status_register();
		interrupt_source = nrf_radio_get_interrupt_source();

		printf("Callback Status: %d\r\n", status);
		printf("Callback Interrupt Source: %d\r\n", interrupt_source);

		//3. Handle the interrupt source
		if(interrupt_source == NRF_RADIO_INT_SRC_RX_DR)
		{
			nrf_radio_retreive_packet_from_fifo();
			nrf_radio_receive_packet_callback((uint8_t *)rx_buffer,rx_packet_length);
			status = NRF_RADIO_INT_SRC_RX_DR_BITPOS;

			//4. Clear the interrupt by writing to STATUS Register
			cmd_buffer[0] = NRF_RADIO_CMD_W_REGISTER_STATUS;
			cmd_buffer[1] = status;
			cmd_packet_length = 2;
			nrf_radio_cmd_write((uint8_t *)cmd_buffer,cmd_packet_length);
		}

		delay_us(1000000);
	}

}

void configure_gpio_led(void)
{
	EnablePeriClk(GPIO_LED_PORT);
	GPIOSetMode(GPIO_LED_PORT,GPIO_LED_PIN,GPIO_MODE_OUTPUT);
}

void gpio_led_turn_on(void)
{
	GPIOWritePin(GPIO_LED_PORT,GPIO_LED_PIN,GPIO_HIGH);
}

void gpio_led_turn_off(void)
{
	GPIOWritePin(GPIO_LED_PORT,GPIO_LED_PIN,GPIO_LOW);
}
