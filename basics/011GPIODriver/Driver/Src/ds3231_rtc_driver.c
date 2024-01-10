/*
 * 	ds3231_rtc_driver.c
 *	Driver Code for the DS3231 RTC Module from Maxim
 *	Reference: https://www.analog.com/media/en/technical-documentation/data-sheets/ds3231.pdf
 *  Created on: 08-Jan-2024
 *      Author: naveen
 */

#include "ds3231_rtc_driver.h"
#include "rtc_driver.h"
#include "common_utils.h"
#include "external_interrupt_config.h"

extern uint32_t *I2C_DEVICE_RTC;
extern uint8_t SlaveAddressRTC;

void RTC_DS3231_Config_Calendar(struct RTC_DS3231_Config_t *pRTCConfig)
{
	uint8_t Tx_Buf[5];
	struct Time ds3231_time;
	struct Date ds3231_date;

	//Configuring the Time Parameters
	memset(&ds3231_time,0,sizeof(struct Time));
	ds3231_time.seconds = (pRTCConfig->RTCClockSecondsUnits << DS3231_RTC_TR_SU) | (pRTCConfig->RTCClockSecondsTens << DS3231_RTC_TR_ST);
	ds3231_time.minutes = (pRTCConfig->RTCClockMinutesUnits << DS3231_RTC_TR_MNU) | (pRTCConfig->RTCClockMinutesTens << DS3231_RTC_TR_MNT);

	if(pRTCConfig->RTCClockHourFormat==DS3231_RTC_HOUR_FORMAT_24)
	{
		ds3231_time.hours = (pRTCConfig->RTCClockHoursUnits << DS3231_RTC_TR_HU) | (pRTCConfig->RTCClockHoursTens << DS3231_RTC_TR_HT) | (pRTCConfig->RTCClockHourFormat << DS3231_RTC_TR_HOUR_FORMAT);
	}
	else if(pRTCConfig->RTCClockHourFormat==DS3231_RTC_HOUR_FORMAT_12)
	{
		ds3231_time.hours = (pRTCConfig->RTCClockHoursUnits << DS3231_RTC_TR_HU) |  (pRTCConfig->RTCClockHoursTens << DS3231_RTC_TR_HT) | (pRTCConfig->RTCClockAMPM << DS3231_RTC_TR_AMPM) | (pRTCConfig->RTCClockHourFormat << DS3231_RTC_TR_HOUR_FORMAT);
	}

	Tx_Buf[0] = 0x00;
	Tx_Buf[1] = ds3231_time.seconds;
	Tx_Buf[2] = ds3231_time.minutes;
	Tx_Buf[3] = ds3231_time.hours;
	I2CMasterSendData(I2C_DEVICE_RTC,Tx_Buf,4,SlaveAddressRTC);
	delay_us(2000);

	//Configuring the Data Parameters in the Device
	memset(&ds3231_date,0,sizeof(struct Date));
	ds3231_date.dayofweek = (pRTCConfig->RTCClockDayOfWeek << DS3231_RTC_DR_WDU);
	ds3231_date.date = (pRTCConfig->RTCClockDateUnits << DS3231_RTC_DR_DU) | (pRTCConfig->RTCClockDateTens << DS3231_RTC_DR_DT);
	ds3231_date.month = (pRTCConfig->RTCClockMonthUnits << DS3231_RTC_DR_MU) | (pRTCConfig->RTCClockMonthTens << DS3231_RTC_DR_MT);
	ds3231_date.year = (pRTCConfig->RTCClockYearUnits << DS3231_RTC_DR_YU) | (pRTCConfig->RTCClockYearTens << DS3231_RTC_DR_YT);
	Tx_Buf[0] = 0x03;
	Tx_Buf[1] = ds3231_date.dayofweek;
	Tx_Buf[2] = ds3231_date.date;
	Tx_Buf[3] = ds3231_date.month;
	Tx_Buf[4] = ds3231_date.year;
	I2CMasterSendData(I2C_DEVICE_RTC,Tx_Buf,5,SlaveAddressRTC);

	return;
}

void RTC_DS3231_Read_Calendar(struct Date *date, struct Time *time)
{
	uint8_t RxBuf[7];

	//Read the current date and time from the DS3231 RTC Module
	I2CMaster_DS3231_RTC_Read(I2C_DEVICE_RTC,RxBuf,7,0, SlaveAddressRTC);

	time->seconds = ((RxBuf[0] >> DS3231_RTC_TR_ST) & 0x7)*10 + ((RxBuf[0] >> DS3231_RTC_TR_SU) & 0xF);
	time->minutes = ((RxBuf[1] >> DS3231_RTC_TR_MNT) & 0x7)*10 + ((RxBuf[1] >> DS3231_RTC_TR_MNU) & 0xF);
	time->hours = ((RxBuf[2] >> DS3231_RTC_TR_HT) & 0x3)*10 + ((RxBuf[2] >> DS3231_RTC_TR_HU) & 0xF);

	date->dayofweek = (RxBuf[3] & 0x7);
	date->date = ((RxBuf[4] >> DS3231_RTC_DR_DT) & 0x3)*10 + ((RxBuf[4] >> DS3231_RTC_DR_DU) & 0xF);
	date->month = ((RxBuf[5] >> DS3231_RTC_DR_MT) & 0x1)*10 + ((RxBuf[5] >> DS3231_RTC_DR_MU) & 0xF);
	date->year = ((RxBuf[6] >> DS3231_RTC_DR_YT) & 0xF)*10 + ((RxBuf[6] >> DS3231_RTC_DR_YU) & 0xF);

	return;
}

void RTC_DS3231_Config_Alarm(struct RTC_DS3231_AlarmConfig_t *pRTCAlarmConfig)
{
	uint8_t Tx_Buf[5];
	uint8_t seconds, minutes, hours, date, control_register;

	seconds = (pRTCAlarmConfig->RTCAlarmSecondsUnits << DS3231_RTC_ALRMR_SU) | (pRTCAlarmConfig->RTCAlarmSecondsTens << DS3231_RTC_ALRMR_ST) | (pRTCAlarmConfig->RTCAlarmConsiderSeconds << DS3231_RTC_ALRMR_MSK1);
	minutes = (pRTCAlarmConfig->RTCAlarmMinutesUnits << DS3231_RTC_ALRMR_MNU) | (pRTCAlarmConfig->RTCAlarmMinutesTens << DS3231_RTC_ALRMR_MNT) | (pRTCAlarmConfig->RTCAlarmConsiderMinutes << DS3231_RTC_ALRMR_MSK2);

	if(pRTCAlarmConfig->RTCAlarmHourFormat == DS3231_RTC_HOUR_FORMAT_24)
	{
		hours = (pRTCAlarmConfig->RTCAlarmHoursUnits << DS3231_RTC_ALRMR_HU) |  (pRTCAlarmConfig->RTCAlarmHoursTens << DS3231_RTC_ALRMR_HT) | (pRTCAlarmConfig->RTCAlarmHourFormat << DS3231_RTC_ALRMR_HOUR_FORMAT) | (pRTCAlarmConfig->RTCAlarmConsiderHours << DS3231_RTC_ALRMR_MSK3);
	}
	else if(pRTCAlarmConfig->RTCAlarmHourFormat == DS3231_RTC_HOUR_FORMAT_12)
	{
		hours = (pRTCAlarmConfig->RTCAlarmHoursUnits << DS3231_RTC_ALRMR_HU) |  (pRTCAlarmConfig->RTCAlarmHoursTens << DS3231_RTC_ALRMR_HT) | (pRTCAlarmConfig->RTCAlarmAMPM << DS3231_RTC_ALRMR_AMPM) | (pRTCAlarmConfig->RTCAlarmHourFormat << DS3231_RTC_ALRMR_HOUR_FORMAT) | (pRTCAlarmConfig->RTCAlarmConsiderHours << DS3231_RTC_ALRMR_MSK3);
	}

	if(pRTCAlarmConfig->RTCAlarmWeekDaySelection == RTC_ALARM_WDSEL_NO)
	{
		//Configure the Date
		date = (pRTCAlarmConfig->RTCAlarmDateUnits << DS3231_RTC_ALRMR_DU) | (pRTCAlarmConfig->RTCAlarmDateTens << DS3231_RTC_ALRMR_DT) | (0 << DS3231_RTC_ALRMR_WDSEL) | (pRTCAlarmConfig->RTCAlarmConsiderDate << DS3231_RTC_ALRMR_MSK4);
	}
	else if(pRTCAlarmConfig->RTCAlarmWeekDaySelection == RTC_ALARM_WDSEL_YES)
	{
		//Configure the WeekDay
		date = (pRTCAlarmConfig->RTCAlarmDayOfWeek << DS3231_RTC_ALRMR_DU) | (1 << DS3231_RTC_ALRMR_WDSEL) | (pRTCAlarmConfig->RTCAlarmConsiderDate << DS3231_RTC_ALRMR_MSK4);
	}

	//Configure the ALARM Parameters in the Device
	if(pRTCAlarmConfig->RTCAlarmSelection == DS3231_RTC_ALARM_1)
	{
		Tx_Buf[0] = 0x07;
		Tx_Buf[1] = seconds;
		Tx_Buf[2] = minutes;
		Tx_Buf[3] = hours;
		Tx_Buf[4] = date;
		I2CMasterSendData(I2C_DEVICE_RTC,Tx_Buf,5,SlaveAddressRTC);
	}
	else if(pRTCAlarmConfig->RTCAlarmSelection == DS3231_RTC_ALARM_2)
	{
		Tx_Buf[0] = 0x0B;
		Tx_Buf[1] = minutes;
		Tx_Buf[2] = hours;
		Tx_Buf[3] = date;
		I2CMasterSendData(I2C_DEVICE_RTC,Tx_Buf,4,SlaveAddressRTC);
	}

	//Enable the Alarm in the configuration register
	//a. Read the configuration register
	I2CMaster_DS3231_RTC_Read(I2C_DEVICE_RTC,&control_register,1,0x0E, SlaveAddressRTC);

	//b. Modify the Alarm Enable Bits
	if(pRTCAlarmConfig->RTCAlarmSelection == DS3231_RTC_ALARM_1)
	{
		control_register |= (1 << DS3231_RTC_CR_A1IE);
	}
	else if(pRTCAlarmConfig->RTCAlarmSelection == DS3231_RTC_ALARM_2)
	{
		control_register |= (1 << DS3231_RTC_CR_A2IE);
	}

	//c. Write to the configuration register
	Tx_Buf[0] = 0x0E;
	Tx_Buf[1] = control_register;
	I2CMasterSendData(I2C_DEVICE_RTC,Tx_Buf,2,SlaveAddressRTC);

	return;
}

void RTC_DS3231_Config_Alarm_Interrupt(void)
{
	configure_external_gpio_interrupt(DS3231_RTC_ALARM_INPUT_GPIO_PORT,DS3231_RTC_ALARM_INPUT_GPIO_PIN,EXTI_FALLING_TRIGGER,DS3231_RTC_ALARM_INPUT_IRQ_NUM);

	return;
}

void RTC_DS3231_Config_Alarm_Output_GPIO(void)
{
	EnablePeriClk(DS3231_RTC_ALARM_OUTPUT_GPIO_PORT);
	GPIOSetMode(DS3231_RTC_ALARM_OUTPUT_GPIO_PORT,DS3231_RTC_ALARM_OUTPUT_GPIO_PIN,GPIO_MODE_OUTPUT);
	GPIOWritePin(DS3231_RTC_ALARM_OUTPUT_GPIO_PORT,DS3231_RTC_ALARM_OUTPUT_GPIO_PIN,GPIO_LOW);

	return;
}

void RTC_DS3231_Config_Button_Interrupt(void)
{
	configure_external_gpio_interrupt(BTN_INT_GPIO_PORT,BTN_INT_GPIO_PIN,EXTI_FALLING_TRIGGER,BTN_INT_IRQ_NUM);

	return;
}
