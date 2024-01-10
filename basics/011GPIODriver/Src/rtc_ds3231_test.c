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

struct Date date;
struct Time time;

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);

int main(void)
{

	struct RTC_DS3231_Config_t ds3231_rtc_config;
	struct RTC_DS3231_AlarmConfig_t ds3231_rtc_alarm_config;


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
	ds3231_rtc_config.RTCClockMinutesUnits = 3;
	ds3231_rtc_config.RTCClockMinutesTens = 4;
	ds3231_rtc_config.RTCClockHoursUnits = 4;
	ds3231_rtc_config.RTCClockHoursTens = 1;
	ds3231_rtc_config.RTCClockHourFormat = DS3231_RTC_HOUR_FORMAT_24;
	ds3231_rtc_config.RTCClockAMPM = 0;
	ds3231_rtc_config.RTCClockDateUnits = 0;
	ds3231_rtc_config.RTCClockDateTens = 1;
	ds3231_rtc_config.RTCClockMonthUnits = 1;
	ds3231_rtc_config.RTCClockMonthTens = 0;
	ds3231_rtc_config.RTCClockYearUnits = 4;
	ds3231_rtc_config.RTCClockYearTens = 2;
	ds3231_rtc_config.RTCClockDayOfWeek = 3;

	RTC_DS3231_Config_Calendar(&ds3231_rtc_config);
	delay_us(10000);

	//Configure the DS3231 RTC Alarm
	memset(&ds3231_rtc_alarm_config,0,sizeof(ds3231_rtc_alarm_config));
	ds3231_rtc_alarm_config.RTCAlarmSelection = DS3231_RTC_ALARM_1;
	ds3231_rtc_alarm_config.RTCAlarmSecondsUnits = 0;
	ds3231_rtc_alarm_config.RTCAlarmSecondsTens = 3;
	ds3231_rtc_alarm_config.RTCAlarmConsiderSeconds = RTC_ALARM_CONSIDER_SECS_YES;
	ds3231_rtc_alarm_config.RTCAlarmMinutesUnits = 4;
	ds3231_rtc_alarm_config.RTCAlarmMinutesTens = 4;
	ds3231_rtc_alarm_config.RTCAlarmConsiderMinutes = RTC_ALARM_CONSIDER_MINS_YES;
	ds3231_rtc_alarm_config.RTCAlarmHoursUnits = 4;
	ds3231_rtc_alarm_config.RTCAlarmHoursTens = 1;
	ds3231_rtc_alarm_config.RTCAlarmHourFormat = DS3231_RTC_HOUR_FORMAT_24;
	ds3231_rtc_alarm_config.RTCAlarmAMPM = 0;
	ds3231_rtc_alarm_config.RTCAlarmConsiderHours = RTC_ALARM_CONSIDER_HRS_YES;
	ds3231_rtc_alarm_config.RTCAlarmDateUnits = 0;
	ds3231_rtc_alarm_config.RTCAlarmDateTens = 1;
	ds3231_rtc_alarm_config.RTCAlarmDayOfWeek = RTC_CLK_DOW_WED;
	ds3231_rtc_alarm_config.RTCAlarmWeekDaySelection = RTC_ALARM_WDSEL_NO;
	ds3231_rtc_alarm_config.RTCAlarmConsiderDate = RTC_ALARM_CONSIDER_DATE_NO;

	RTC_DS3231_Config_Alarm(&ds3231_rtc_alarm_config);
	RTC_DS3231_Config_Alarm_Interrupt();

	//Configure the Alarm Output GPIO Pin
	RTC_DS3231_Config_Alarm_Output_GPIO();

	//Configure the Button Interrupt
	RTC_DS3231_Config_Button_Interrupt();

	delay_us(10000);

	while(1)
	{
		RTC_DS3231_Read_Calendar(&date,&time);
		RTC_Display_Calendar_LCD(&date,&time);
	}

	return 0;
}

void EXTI0_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	//Handling the Interrupt
	GPIOWritePin(DS3231_RTC_ALARM_OUTPUT_GPIO_PORT,DS3231_RTC_ALARM_OUTPUT_GPIO_PIN,GPIO_HIGH);

	//Clearing the Button Interrupt
	*pEXTI_PR |= (1 << DS3231_RTC_ALARM_INPUT_GPIO_PIN);		// Clearing the EXTI_PR Register

	return;
}

void EXTI2_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	//1. Calling the RTC Alarm Interrupt Callback function
	GPIOWritePin(DS3231_RTC_ALARM_OUTPUT_GPIO_PORT,DS3231_RTC_ALARM_OUTPUT_GPIO_PIN,GPIO_LOW);

	//2. Clearing the Interrupt
	*pEXTI_PR |= (1 << BTN_INT_GPIO_PIN);		// Clearing the EXTI_PR Register

	return;
}
