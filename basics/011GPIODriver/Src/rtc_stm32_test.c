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
#include "common_utils.h"
#include "external_interrupt_config.h"

#define RTC_ALARM_GPIO_PORT		GPIOD
#define RTC_ALARM_GPIO_PIN		GPIO_PIN_15

struct Date date;
struct Time time;

void RTC_Alarm_Interrupt_Callback(void);
void EXTI0_IRQHandler(void);

int main(void)
{

	struct RTC_Config_t stm32_rtc_config;
	struct RTC_AlarmConfig_t stm32_rtc_alarm_config;


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

	//Configure STM32 RTC Peripheral
	memset(&stm32_rtc_config,0,sizeof(stm32_rtc_config));
	stm32_rtc_config.RTCClockSource = RTC_CLK_SRC_HSE;
	stm32_rtc_config.RTCClockHSEPrescaler = RTC_CLK_PRESCALER_DIV_8;
	stm32_rtc_config.RTCClockPrescalerA = 99;
	stm32_rtc_config.RTCClockPrescalerS = 9999;
	stm32_rtc_config.RTCClockSecondsUnits = 0;
	stm32_rtc_config.RTCClockSecondsTens = 0;
	stm32_rtc_config.RTCClockMinutesUnits = 5;
	stm32_rtc_config.RTCClockMinutesTens = 3;
	stm32_rtc_config.RTCClockHoursUnits = 7;
	stm32_rtc_config.RTCClockHoursTens = 0;
	stm32_rtc_config.RTCClockAMPM = 0;
	stm32_rtc_config.RTCClockDateUnits = 7;
	stm32_rtc_config.RTCClockDateTens = 0;
	stm32_rtc_config.RTCClockMonthUnits = 1;
	stm32_rtc_config.RTCClockMonthTens = 0;
	stm32_rtc_config.RTCClockYearUnits = 4;
	stm32_rtc_config.RTCClockYearTens = 2;
	stm32_rtc_config.RTCClockDayOfWeek = 7;

	RTC_Config_Calendar(&stm32_rtc_config);
	delay_us(10000);

	//Configure the STM32 RTC Alarm
	memset(&stm32_rtc_alarm_config,0,sizeof(stm32_rtc_alarm_config));
	stm32_rtc_alarm_config.RTCAlarmSelection = RTC_ALARM_A;
	stm32_rtc_alarm_config.RTCAlarmSecondsUnits = 0;
	stm32_rtc_alarm_config.RTCAlarmSecondsTens = 0;
	stm32_rtc_alarm_config.RTCAlarmConsiderSeconds = RTC_ALARM_CONSIDER_SECS_YES;
	stm32_rtc_alarm_config.RTCAlarmMinutesUnits = 7;
	stm32_rtc_alarm_config.RTCAlarmMinutesTens = 3;
	stm32_rtc_alarm_config.RTCAlarmConsiderMinutes = RTC_ALARM_CONSIDER_MINS_YES;
	stm32_rtc_alarm_config.RTCAlarmHoursUnits = 7;
	stm32_rtc_alarm_config.RTCAlarmHoursTens = 0;
	stm32_rtc_alarm_config.RTCAlarmAMPM = 0;
	stm32_rtc_alarm_config.RTCAlarmConsiderHours = RTC_ALARM_CONSIDER_HRS_YES;
	stm32_rtc_alarm_config.RTCAlarmDateUnits = 7;
	stm32_rtc_alarm_config.RTCAlarmDateTens = 0;
	stm32_rtc_alarm_config.RTCAlarmWeekDaySelection = RTC_ALARM_WDSEL_NO;
	stm32_rtc_alarm_config.RTCAlarmConsiderDate = RTC_ALARM_CONSIDER_DATE_NO;

	RTC_Config_Alarm(&stm32_rtc_alarm_config);
	RTC_Config_Alarm_Interrupt();

	//Configure the GPIO Related to RTC Alarm
	EnablePeriClk(RTC_ALARM_GPIO_PORT);
	GPIOSetMode(RTC_ALARM_GPIO_PORT,RTC_ALARM_GPIO_PIN,GPIO_MODE_OUTPUT);
	GPIOWritePin(RTC_ALARM_GPIO_PORT,RTC_ALARM_GPIO_PIN,GPIO_LOW);

	//Configure the Button Interrupt
	configure_external_interrupt();

	delay_us(10000);

	while(1)
	{
		RTC_Read_Calendar(&date,&time);
		RTC_Display_Calendar_LCD(&date,&time);
	}

	return 0;
}

void RTC_Alarm_Interrupt_Callback(void)
{
	GPIOWritePin(RTC_ALARM_GPIO_PORT,RTC_ALARM_GPIO_PIN,GPIO_HIGH);

	return;
}

void EXTI0_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	//Handling the Interrupt
	GPIOWritePin(RTC_ALARM_GPIO_PORT,RTC_ALARM_GPIO_PIN,GPIO_LOW);

	//Clearing the Button Interrupt
	*pEXTI_PR |= (1 << INTERRUPT_PIN);		// Clearing the EXTI_PR Register

	return;
}
