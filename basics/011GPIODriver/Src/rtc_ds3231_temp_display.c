/*
 * rtc_stm32_test.c
 *
 *  Created on: 06-Jan-2024
 *      Author: naveen
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "rtc_driver.h"
#include "ds3231_rtc_driver.h"
#include "common_utils.h"
#include "external_interrupt_config.h"

int main(void)
{
	struct RTC_DS3231_Config_t ds3231_rtc_config;
	uint32_t count = 0;

	//Configure the Timer
	configure_delay_timer();

	//Configure the LCD Display
	//a. Configure the I2C Peripheral
	configure_i2c();

	//b. Disable printf() buffering
	setbuf(stdout, NULL);

	//c. Initialize the LCD Display
	delay_us(1000000);
	lcd_pcf8574_init();
	delay_us(100000);

	//Configure DS3231 RTC Peripheral
	//a. Configure the I2C Port for the RTC
	configure_i2c_rtc();

	//b. Configure the parameters of the RTC
	memset(&ds3231_rtc_config,0,sizeof(ds3231_rtc_config));
	ds3231_rtc_config.RTCClockSecondsUnits = 0;
	ds3231_rtc_config.RTCClockSecondsTens = 0;
	ds3231_rtc_config.RTCClockMinutesUnits = 0;
	ds3231_rtc_config.RTCClockMinutesTens = 1;
	ds3231_rtc_config.RTCClockHoursUnits = 2;
	ds3231_rtc_config.RTCClockHoursTens = 0;
	ds3231_rtc_config.RTCClockHourFormat = DS3231_RTC_HOUR_FORMAT_12;
	ds3231_rtc_config.RTCClockAMPM = DS3231_RTC_HOUR_PM;
	ds3231_rtc_config.RTCClockDateUnits = 1;
	ds3231_rtc_config.RTCClockDateTens = 1;
	ds3231_rtc_config.RTCClockMonthUnits = 1;
	ds3231_rtc_config.RTCClockMonthTens = 0;
	ds3231_rtc_config.RTCClockYearUnits = 4;
	ds3231_rtc_config.RTCClockYearTens = 2;
	ds3231_rtc_config.RTCClockDayOfWeek = RTC_CLK_DOW_THU;

	RTC_DS3231_Config_Calendar(&ds3231_rtc_config);
	delay_us(1000000);

	while(1)
	{
		RTC_DS3231_Display_Temp_LCD();
		lcd_pcf8574_set_position(1,0);
		delay_us(500);
		printf("Iteration: %ld", ++count);
		delay_us(64000000);
	}

	return 0;
}