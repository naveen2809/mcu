/*
 * ds3231_rtc_driver.h
 * Driver Code for the DS3231 RTC Module from Maxim
 * Reference: https://www.analog.com/media/en/technical-documentation/data-sheets/ds3231.pdf
 * Created on: 08-Jan-2024
 *      Author: naveen
 */

#ifndef INC_DS3231_RTC_DRIVER_H_
#define INC_DS3231_RTC_DRIVER_H_

#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "rtc_driver.h"

struct RTC_DS3231_Config_t
{
	uint8_t RTCClockSecondsUnits;
	uint8_t RTCClockSecondsTens;
	uint8_t RTCClockMinutesUnits;
	uint8_t RTCClockMinutesTens;
	uint8_t RTCClockHoursUnits;
	uint8_t RTCClockHoursTens;
	uint8_t RTCClockHourFormat;
	uint8_t RTCClockAMPM;
	uint8_t RTCClockDateUnits;
	uint8_t RTCClockDateTens;
	uint8_t RTCClockMonthUnits;
	uint8_t RTCClockMonthTens;
	uint8_t RTCClockYearUnits;
	uint8_t RTCClockYearTens;
	uint8_t RTCClockDayOfWeek;
};

struct RTC_DS3231_AlarmConfig_t
{
	uint8_t RTCAlarmSelection;
	uint8_t RTCAlarmSecondsUnits;
	uint8_t RTCAlarmSecondsTens;
	uint8_t RTCAlarmConsiderSeconds;
	uint8_t RTCAlarmMinutesUnits;
	uint8_t RTCAlarmMinutesTens;
	uint8_t RTCAlarmConsiderMinutes;
	uint8_t RTCAlarmHoursUnits;
	uint8_t RTCAlarmHoursTens;
	uint8_t RTCAlarmHourFormat;
	uint8_t RTCAlarmAMPM;
	uint8_t RTCAlarmConsiderHours;
	uint8_t RTCAlarmDateUnits;
	uint8_t RTCAlarmDateTens;
	uint8_t RTCAlarmDayOfWeek;
	uint8_t RTCAlarmWeekDaySelection;
	uint8_t RTCAlarmConsiderDate;
};

#define DS3231_RTC_TR_SU					0
#define DS3231_RTC_TR_ST					4
#define DS3231_RTC_TR_MNU					0
#define DS3231_RTC_TR_MNT					4
#define DS3231_RTC_TR_HU					0
#define DS3231_RTC_TR_HT					4
#define DS3231_RTC_TR_AMPM					5
#define DS3231_RTC_TR_HOUR_FORMAT			6

#define DS3231_RTC_DR_DU					0
#define DS3231_RTC_DR_DT					4
#define DS3231_RTC_DR_MU					0
#define DS3231_RTC_DR_MT					4
#define DS3231_RTC_DR_WDU					0
#define DS3231_RTC_DR_YU					0
#define DS3231_RTC_DR_YT					4

#define DS3231_RTC_ALRMR_SU					0
#define DS3231_RTC_ALRMR_ST					4
#define DS3231_RTC_ALRMR_MSK1				7
#define DS3231_RTC_ALRMR_MNU				0
#define DS3231_RTC_ALRMR_MNT				4
#define DS3231_RTC_ALRMR_MSK2				7
#define DS3231_RTC_ALRMR_HU					0
#define DS3231_RTC_ALRMR_HT					4
#define DS3231_RTC_ALRMR_AMPM				5
#define DS3231_RTC_ALRMR_HOUR_FORMAT		6
#define DS3231_RTC_ALRMR_MSK3				7
#define DS3231_RTC_ALRMR_DU					0
#define DS3231_RTC_ALRMR_DT					4
#define DS3231_RTC_ALRMR_WDSEL				6
#define DS3231_RTC_ALRMR_MSK4				7

#define DS3231_RTC_HOUR_FORMAT_24			0
#define DS3231_RTC_HOUR_FORMAT_12			1
#define DS3231_RTC_HOUR_AM					0
#define DS3231_RTC_HOUR_PM					1

#define DS3231_RTC_ALARM_1					0
#define DS3231_RTC_ALARM_2					1

#define DS3231_RTC_CR_EOSC					7
#define DS3231_RTC_CR_BBSQW					6
#define DS3231_RTC_CR_CONV					5
#define DS3231_RTC_CR_RS2					4
#define DS3231_RTC_CR_RS1					3
#define DS3231_RTC_CR_INTCN					2
#define DS3231_RTC_CR_A2IE					1
#define DS3231_RTC_CR_A1IE					0

#define DS3231_RTC_CSR_OSF					7
#define DS3231_RTC_CSR_EN32KHz				3
#define DS3231_RTC_CSR_BSY					2
#define DS3231_RTC_CSR_A2F					1
#define DS3231_RTC_CSR_A1F					0

//DS3231 ALARM Input Parameters
#define DS3231_RTC_ALARM_INPUT_GPIO_PORT	GPIOC
#define DS3231_RTC_ALARM_INPUT_GPIO_PIN		GPIO_PIN_10
#define DS3231_RTC_ALARM_INPUT_IRQ_NUM		40

//DS3231 ALARM Output GPIO Parameters
#define DS3231_RTC_ALARM_OUTPUT_GPIO_PORT	GPIOD
#define DS3231_RTC_ALARM_OUTPUT_GPIO_PIN	GPIO_PIN_15

//Button Parameters
#define BTN_INT_GPIO_PORT   GPIOD
#define BTN_INT_GPIO_PIN    GPIO_PIN_2
#define BTN_INT_IRQ_NUM  	8

void RTC_DS3231_Config_Calendar(struct RTC_DS3231_Config_t *pRTCConfig);
void RTC_DS3231_Read_Calendar(struct Date *date, struct Time *time);
void RTC_DS3231_Display_Calendar_LCD(struct Date *date, struct Time *time);
void RTC_DS3231_Config_Alarm(struct RTC_DS3231_AlarmConfig_t *pRTCAlarmConfig);
uint8_t RTC_DS3231_Read_Register(uint8_t address);
void RTC_DS3231_Config_Alarm_Interrupt(void);
void RTC_DS3231_Config_Alarm_Output_GPIO(void);
void RTC_DS3231_Config_Button_Interrupt(void);
void RTC_DS3231_Clear_Alarm(uint8_t AlarmSelection);

#endif /* INC_DS3231_RTC_DRIVER_H_ */
