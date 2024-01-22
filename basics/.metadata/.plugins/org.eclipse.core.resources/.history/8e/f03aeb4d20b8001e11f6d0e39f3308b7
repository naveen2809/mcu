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

extern uint32_t *I2C_DEVICE_RTC;
extern uint8_t SlaveAddressRTC;

static void disable_interrupts(void);
static void enable_interrupts(void);

int main(void)
{

	struct RTC_DS3231_Config_t ds3231_rtc_config;
	struct RTC_DS3231_AlarmConfig_t ds3231_rtc_alarm_config;

	disable_interrupts();

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
	ds3231_rtc_config.RTCClockMinutesUnits = 6;
	ds3231_rtc_config.RTCClockMinutesTens = 3;
	ds3231_rtc_config.RTCClockHoursUnits = 9;
	ds3231_rtc_config.RTCClockHoursTens = 0;
	ds3231_rtc_config.RTCClockHourFormat = DS3231_RTC_HOUR_FORMAT_12;
	ds3231_rtc_config.RTCClockAMPM = DS3231_RTC_HOUR_AM;
	ds3231_rtc_config.RTCClockDateUnits = 1;
	ds3231_rtc_config.RTCClockDateTens = 2;
	ds3231_rtc_config.RTCClockMonthUnits = 1;
	ds3231_rtc_config.RTCClockMonthTens = 0;
	ds3231_rtc_config.RTCClockYearUnits = 4;
	ds3231_rtc_config.RTCClockYearTens = 2;
	ds3231_rtc_config.RTCClockDayOfWeek = RTC_CLK_DOW_SUN;

	RTC_DS3231_Config_Calendar(&ds3231_rtc_config);
	delay_us(100000);

	//Configure the DS3231 RTC Alarm 1
	memset(&ds3231_rtc_alarm_config,0,sizeof(ds3231_rtc_alarm_config));
	ds3231_rtc_alarm_config.RTCAlarmSelection = DS3231_RTC_ALARM_1;
	ds3231_rtc_alarm_config.RTCAlarmSecondsUnits = 0;
	ds3231_rtc_alarm_config.RTCAlarmSecondsTens = 0;
	ds3231_rtc_alarm_config.RTCAlarmConsiderSeconds = RTC_ALARM_CONSIDER_SECS_NO;
	ds3231_rtc_alarm_config.RTCAlarmMinutesUnits = 0;
	ds3231_rtc_alarm_config.RTCAlarmMinutesTens = 0;
	ds3231_rtc_alarm_config.RTCAlarmConsiderMinutes = RTC_ALARM_CONSIDER_MINS_NO;
	ds3231_rtc_alarm_config.RTCAlarmHoursUnits = 0;
	ds3231_rtc_alarm_config.RTCAlarmHoursTens = 0;
	ds3231_rtc_alarm_config.RTCAlarmHourFormat = DS3231_RTC_HOUR_FORMAT_12;
	ds3231_rtc_alarm_config.RTCAlarmAMPM = DS3231_RTC_HOUR_PM;
	ds3231_rtc_alarm_config.RTCAlarmConsiderHours = RTC_ALARM_CONSIDER_HRS_NO;
	ds3231_rtc_alarm_config.RTCAlarmDateUnits = 0;
	ds3231_rtc_alarm_config.RTCAlarmDateTens = 0;
	ds3231_rtc_alarm_config.RTCAlarmDayOfWeek = RTC_CLK_DOW_FRI;
	ds3231_rtc_alarm_config.RTCAlarmWeekDaySelection = RTC_ALARM_WDSEL_NO;
	ds3231_rtc_alarm_config.RTCAlarmConsiderDate = RTC_ALARM_CONSIDER_DATE_NO;

	RTC_DS3231_Config_Alarm(&ds3231_rtc_alarm_config);
	RTC_DS3231_Clear_Alarm(DS3231_RTC_ALARM_1);

	//Configure the DS3231 RTC Alarm 2
	memset(&ds3231_rtc_alarm_config,0,sizeof(ds3231_rtc_alarm_config));
	ds3231_rtc_alarm_config.RTCAlarmSelection = DS3231_RTC_ALARM_2;
	ds3231_rtc_alarm_config.RTCAlarmMinutesUnits = 8;
	ds3231_rtc_alarm_config.RTCAlarmMinutesTens = 3;
	ds3231_rtc_alarm_config.RTCAlarmConsiderMinutes = RTC_ALARM_CONSIDER_MINS_YES;
	ds3231_rtc_alarm_config.RTCAlarmHoursUnits = 9;
	ds3231_rtc_alarm_config.RTCAlarmHoursTens = 0;
	ds3231_rtc_alarm_config.RTCAlarmHourFormat = DS3231_RTC_HOUR_FORMAT_12;
	ds3231_rtc_alarm_config.RTCAlarmAMPM = DS3231_RTC_HOUR_AM;
	ds3231_rtc_alarm_config.RTCAlarmConsiderHours = RTC_ALARM_CONSIDER_HRS_YES;
	ds3231_rtc_alarm_config.RTCAlarmDateUnits = 1;
	ds3231_rtc_alarm_config.RTCAlarmDateTens = 2;
	ds3231_rtc_alarm_config.RTCAlarmDayOfWeek = RTC_CLK_DOW_SUN;
	ds3231_rtc_alarm_config.RTCAlarmWeekDaySelection = RTC_ALARM_WDSEL_NO;
	ds3231_rtc_alarm_config.RTCAlarmConsiderDate = RTC_ALARM_CONSIDER_DATE_NO;

	RTC_DS3231_Config_Alarm(&ds3231_rtc_alarm_config);
	RTC_DS3231_Clear_Alarm(DS3231_RTC_ALARM_2);

	//Configure the Alarm Input GPIO Pin and Enable the Interrupt in NVIC
	RTC_DS3231_Config_Alarm_Interrupt();

	//Configure the Alarm Output GPIO Pin
	RTC_DS3231_Config_Alarm_Output_GPIO();

	//Configure the Button Interrupt
	RTC_DS3231_Config_Button_Interrupt();

	delay_us(10000);

	enable_interrupts();

	while(1)
	{

	}

	return 0;
}

//Alarm Interrupt
void EXTI15_10_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;
	uint8_t reg_value, reg_address;
	uint8_t control_status_reg_address = 15;
	uint8_t Tx_Buf[2];

	disable_interrupts();

	//Reading the DS3231 RTC Module Status Register
	reg_address = 0xF;										//Address of Control and Status Register
	reg_value = RTC_DS3231_Read_Register(reg_address);

	delay_us(1000);

	if(reg_value & (1 << DS3231_RTC_CSR_A1F))				//Checking if Alarm 1 Flag is Set
	{
		//Update the LCD
		RTC_DS3231_Read_Calendar(&date,&time);
		RTC_DS3231_Display_Calendar_LCD(&date,&time);
	}

	if(reg_value & (1 << DS3231_RTC_CSR_A2F))				//Checking if Alarm 2 Flag is Set
	{
		//Turning on the Output LED
		GPIOWritePin(DS3231_RTC_ALARM_OUTPUT_GPIO_PORT,DS3231_RTC_ALARM_OUTPUT_GPIO_PIN,GPIO_HIGH);
	}

	//Clearing the Alarm Flags in the DS3231 RTC Module
	Tx_Buf[0] = control_status_reg_address;
	Tx_Buf[1] = reg_value & 0xFC;
	I2CMasterSendData(I2C_DEVICE_RTC,Tx_Buf,2,SlaveAddressRTC);
	delay_us(2000);

	//Clearing the Alarm Interrupt
	*pEXTI_PR |= (1 << DS3231_RTC_ALARM_INPUT_GPIO_PIN);	//Clearing the EXTI_PR Register

	enable_interrupts();

	return;
}

//Button Interrupt
void EXTI2_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	disable_interrupts();

	GPIOWritePin(DS3231_RTC_ALARM_OUTPUT_GPIO_PORT,DS3231_RTC_ALARM_OUTPUT_GPIO_PIN,GPIO_LOW);

	//2. Clearing the Button Interrupt
	*pEXTI_PR |= (1 << DS3231_RTC_BTN_INT_GPIO_PIN);		// Clearing the EXTI_PR Register

	enable_interrupts();

	return;
}

static void disable_interrupts(void)
{
	__asm volatile ("CPSID I");

	return;
}

static void enable_interrupts(void)
{
	__asm volatile ("CPSIE I");

	return;
}
