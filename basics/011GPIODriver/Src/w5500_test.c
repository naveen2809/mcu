/*
 * w5500_test.c
 *
 *  Created on: 04-May-2024
 *      Author: naveen
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "spi_driver.h"
#include "general_purpose_timer.h"
#include "common_utils.h"
#include "w5500_driver.h"

int main(void)
{
	uint8_t w5500_chip_ver;
	uint8_t w5500_phy_cfgr;
	uint8_t w5500_common_mode_reg;
	uint8_t gw_ip_addr[4] = {192,168,1,1};
	uint8_t nw_sub_mask[4] = {255,255,255,0};
	uint8_t ip_addr[4] = {192,168,1,150};
	uint8_t mac_addr[6] = {0x00,0x02,0x73,0x90,0x11,0x22};

	uint8_t buf[6];
	uint8_t i;

	configure_uart();
	configure_spi();
	w5500_configure_pins();

	w5500_chip_ver = w5500_get_chip_version();
	printf("W5500 Chip Version: 0x%X\r\n",w5500_chip_ver);

	w5500_configure_phy();

	w5500_phy_cfgr = w5500_read_phy_cfgr();
	printf("The current value of PHYCFGR: 0x%X\r\n", w5500_phy_cfgr);

	w5500_common_mode_reg = w5500_read_common_mode_reg();
	printf("The current value of Common Mode Register (MR): 0x%X\r\n", w5500_common_mode_reg);

	w5500_configure_gw_ip_address(gw_ip_addr);
	w5500_configure_subnet_mask(nw_sub_mask);
	w5500_configure_ip_address(ip_addr);
	w5500_configure_mac_address(mac_addr);

	w5500_get_ip_address(buf);
	printf("IP Address: ");
	for(i=0;i<4;i++)
		printf("%d ",buf[i]);
	printf("\r\n");

	w5500_get_subnet_mask(buf);
	printf("Subnet Mask: ");
	for(i=0;i<4;i++)
		printf("%d ",buf[i]);
	printf("\r\n");

	w5500_get_gw_ip_address(buf);
	printf("Gateway IP: ");
	for(i=0;i<4;i++)
		printf("%d ",buf[i]);
	printf("\r\n");

	w5500_get_mac_address(buf);
	printf("MAC Address: ");
	for(i=0;i<6;i++)
		printf("%X ",buf[i]);
	printf("\r\n");

	while(1)
	{

	}

	return 0;
}
