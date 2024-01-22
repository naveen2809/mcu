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

static struct Date saved_date;
static struct Time saved_time;
uint32_t ds3231_display_count = 0;

static void format_dow(struct Date *date,char *dow);
static void format_mon(struct Date *date,char *mon);

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
	if(((time->hours >> DS3231_RTC_TR_HOUR_FORMAT) & 0x1) == 0)     //24 Hour Format
	{
		time->hours = ((RxBuf[2] >> DS3231_RTC_TR_HT) & 0x3)*10 + ((RxBuf[2] >> DS3231_RTC_TR_HU) & 0xF);
		time->hours |= (RxBuf[2] & (1 << DS3231_RTC_TR_HOUR_FORMAT));
	}
	else   //12 Hour Format
	{
		time->hours = ((RxBuf[2] >> DS3231_RTC_TR_HT) & 0x1)*10 + ((RxBuf[2] >> DS3231_RTC_TR_HU) & 0xF);
		time->hours |= (RxBuf[2] & (1 << DS3231_RTC_TR_AMPM));
		time->hours |= (RxBuf[2] & (1 << DS3231_RTC_TR_HOUR_FORMAT));
	}

	date->dayofweek = (RxBuf[3] & 0x7);
	date->date = ((RxBuf[4] >> DS3231_RTC_DR_DT) & 0x3)*10 + ((RxBuf[4] >> DS3231_RTC_DR_DU) & 0xF);
	date->month = ((RxBuf[5] >> DS3231_RTC_DR_MT) & 0x1)*10 + ((RxBuf[5] >> DS3231_RTC_DR_MU) & 0xF);
	date->year = ((RxBuf[6] >> DS3231_RTC_DR_YT) & 0xF)*10 + ((RxBuf[6] >> DS3231_RTC_DR_YU) & 0xF);

	return;
}

void RTC_DS3231_Display_Calendar_LCD(struct Date *date, struct Time *time)
{
	char dow[4];
	char mon[4];

	//Ensure that printf is redirected to LCD is "syscalls.c" file
	if(ds3231_display_count == 0)
	{
		memset(&saved_time,0,sizeof(struct Time));
		memset(&saved_date,0,sizeof(struct Date));

		//Print the Time Information
		if(((time->hours >> DS3231_RTC_TR_HOUR_FORMAT) & 0x1) == 0)      //24 Hour Format
		{
			lcd_pcf8574_clear_screen();
			delay_us(3000);
			lcd_pcf8574_return_home();
			delay_us(3000);
			printf("    %02d:%02d:%02d",time->hours,time->minutes,time->seconds);
		}
		else  //12 Hour Format
		{
			lcd_pcf8574_clear_screen();
			delay_us(3000);
			lcd_pcf8574_return_home();
			delay_us(3000);
			if(((time->hours >> DS3231_RTC_TR_AMPM) & 0x1) == 0)		//AM
			{
				printf("   %02d:%02d:%02d AM",(time->hours & 0xF),time->minutes,time->seconds);
			}
			else
			{
				printf("   %02d:%02d:%02d PM",(time->hours & 0xF),time->minutes,time->seconds);
			}
		}

		//Saving the time information
		saved_time.seconds = time->seconds;
		saved_time.minutes = time->minutes;
		saved_time.hours = time->hours;

		//Print the Date Information
		lcd_pcf8574_set_position(1,0);
		delay_us(3000);
		format_dow(date,dow);
		format_mon(date,mon);
		printf("  %s %02d-%s-%02d",dow,date->date,mon,date->year);

		//Saving the date information
		saved_date.date = date->date;
		saved_date.month = date->month;
		saved_date.year = date->year;
		saved_date.dayofweek = date->dayofweek;

		//Update count value
		ds3231_display_count++;
	}
	else
	{
		if(((time->hours >> DS3231_RTC_TR_HOUR_FORMAT) & 0x1) == 0)   //24 Hour Format
		{
			//Update Seconds
			if(saved_time.seconds != time->seconds)
			{
				lcd_pcf8574_set_position(0,10);
				delay_us(500);
				printf("%02d",time->seconds);
				saved_time.seconds = time->seconds;
			}

			//Update Minutes
			if(saved_time.minutes != time->minutes)
			{
				lcd_pcf8574_set_position(0,7);
				delay_us(500);
				printf("%02d",time->minutes);
				saved_time.minutes = time->minutes;
			}

			//Update Hours
			if(saved_time.hours != time->hours)
			{
				lcd_pcf8574_set_position(0,4);
				delay_us(500);
				printf("%02d",time->hours);
				saved_time.hours = time->hours;
			}
		}
		else    //12 Hour Format
		{
			//Update Seconds
			if(saved_time.seconds != time->seconds)
			{
				lcd_pcf8574_set_position(0,9);
				delay_us(500);
				if(((time->hours >> DS3231_RTC_TR_AMPM) & 0x1) == 0)    //AM
				{
					printf("%02d AM",time->seconds);
				}
				else //PM
				{
					printf("%02d PM",time->seconds);
				}
				saved_time.seconds = time->seconds;
			}

			//Update Minutes
			if(saved_time.minutes != time->minutes)
			{
				lcd_pcf8574_set_position(0,6);
				delay_us(500);
				printf("%02d",time->minutes);
				saved_time.minutes = time->minutes;
			}

			//Update Hours
			if(saved_time.hours != time->hours)
			{
				lcd_pcf8574_set_position(0,3);
				delay_us(500);
				printf("%02d",(time->hours & 0xF));
				saved_time.hours = time->hours;
			}
		}

		//Update Date
		if(saved_date.date != date->date)
		{
			lcd_pcf8574_set_position(1,6);
			delay_us(500);
			printf("%02d",date->date);
			saved_date.date = date->date;
		}

		//Update Month
		if(saved_date.month != date->month)
		{
			lcd_pcf8574_set_position(1,9);
			delay_us(500);
			format_mon(date,mon);
			printf("%s",mon);
			saved_date.month = date->month;
		}

		//Update Year
		if(saved_date.year != date->year)
		{
			lcd_pcf8574_set_position(1,13);
			delay_us(500);
			printf("%02d",date->year);
			saved_date.year = date->year;
		}

		//Update Day of Week
		if(saved_date.dayofweek != date->dayofweek)
		{
			lcd_pcf8574_set_position(1,2);
			delay_us(500);
			format_dow(date,dow);
			printf("%s",dow);
			saved_date.dayofweek = date->dayofweek;
		}
	}

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
	configure_external_gpio_interrupt(DS3231_RTC_BTN_INT_GPIO_PORT,DS3231_RTC_BTN_INT_GPIO_PIN,EXTI_FALLING_TRIGGER,DS3231_RTC_BTN_INT_IRQ_NUM);

	return;
}

uint8_t RTC_DS3231_Read_Register(uint8_t address)
{
	uint8_t reg;

	I2CMaster_DS3231_RTC_Read(I2C_DEVICE_RTC,&reg,1,address,SlaveAddressRTC);

	return reg;
}

void RTC_DS3231_Clear_Alarm(uint8_t AlarmSelection)
{
	uint8_t reg;
	uint8_t control_status_reg_address = 15;
	uint8_t Tx_Buf[2];

	//Read the Control Status Register
	I2CMaster_DS3231_RTC_Read(I2C_DEVICE_RTC,&reg,1,control_status_reg_address,SlaveAddressRTC);

	//Update the Control Status Register
	Tx_Buf[0] = control_status_reg_address;

	if(AlarmSelection == DS3231_RTC_ALARM_1)
	{
		Tx_Buf[1] = reg & 0xFE;
	}
	else if(AlarmSelection == DS3231_RTC_ALARM_2)
	{
		Tx_Buf[1] = reg & 0xFD;
	}

	I2CMasterSendData(I2C_DEVICE_RTC,Tx_Buf,2,SlaveAddressRTC);
	delay_us(2000);

	return;
}

void RTC_DS3231_Display_Temp_LCD(void)
{

	uint8_t temp[2];
	uint8_t temp1_reg_address = 0x11;
	uint8_t temp2_reg_address = 0x12;
	uint8_t temp_fraction, shift_len = 6;

	//Reading the temperature registers
	I2CMaster_DS3231_RTC_Read(I2C_DEVICE_RTC,&temp[0],1,temp1_reg_address,SlaveAddressRTC);
	I2CMaster_DS3231_RTC_Read(I2C_DEVICE_RTC,&temp[1],1,temp2_reg_address,SlaveAddressRTC);

	//Converting the fractional part of temperature to decimal values
	if((temp[1] >> shift_len) == 0)
	{
		temp_fraction = 0;
	}
	else if((temp[1] >> shift_len) == 1)
	{
		temp_fraction = 25;
	}
	else if((temp[1] >> shift_len) == 2)
	{
		temp_fraction = 50;
	}
	else if((temp[1] >> shift_len) == 3)
	{
		temp_fraction = 75;
	}

	lcd_pcf8574_clear_screen();
	delay_us(3000);
	lcd_pcf8574_return_home();
	delay_us(3000);
	printf("TEMP: %02d.%02d C",temp[0],temp_fraction);

	return;
}

static void format_dow(struct Date *date,char *dow)
{
	switch(date->dayofweek)
	{
		case 1:
			strcpy(dow,"Mon");
			break;
		case 2:
			strcpy(dow,"Tue");
			break;
		case 3:
			strcpy(dow,"Wed");
			break;
		case 4:
			strcpy(dow,"Thu");
			break;
		case 5:
			strcpy(dow,"Fri");
			break;
		case 6:
			strcpy(dow,"Sat");
			break;
		case 7:
			strcpy(dow,"Sun");
			break;
		default:
			break;
	}

	return;
}

static void format_mon(struct Date *date,char *mon)
{
	switch(date->month)
	{
		case 1:
			strcpy(mon,"Jan");
			break;
		case 2:
			strcpy(mon,"Feb");
			break;
		case 3:
			strcpy(mon,"Mar");
			break;
		case 4:
			strcpy(mon,"Apr");
			break;
		case 5:
			strcpy(mon,"May");
			break;
		case 6:
			strcpy(mon,"Jun");
			break;
		case 7:
			strcpy(mon,"Jul");
			break;
		case 8:
			strcpy(mon,"Aug");
			break;
		case 9:
			strcpy(mon,"Sep");
			break;
		case 10:
			strcpy(mon,"Oct");
			break;
		case 11:
			strcpy(mon,"Nov");
			break;
		case 12:
			strcpy(mon,"Dec");
			break;
		default:
			break;
	}

	return;
}
