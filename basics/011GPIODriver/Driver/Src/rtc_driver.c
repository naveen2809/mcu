/*
 * rtc_driver.c
 *
 *  Created on: 05-Jan-2024
 *      Author: naveen
 */

#include "rtc_driver.h"
#include "common_utils.h"
#include "external_interrupt_config.h"

uint32_t *pRCC_CR = (uint32_t *) RCC_CR_REG_ADDR;
uint32_t *pRCC_CFGR = (uint32_t *) RCC_CFGR_REG_ADDR;
uint32_t *pRCC_BDCR = (uint32_t *) RCC_BDCR_REG_ADDR;
uint32_t *pPWR_CR = (uint32_t *) PWR_CR_REG_ADDR;
uint32_t *pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;

struct RTC_RegDef_t * stm32_rtc = (struct RTC_RegDef_t *) RTC_BASE_ADDR;

extern void RTC_Alarm_Interrupt_Callback(void);
static void format_dow(struct Date *date,char *dow);
static void format_mon(struct Date *date,char *mon);

void RTC_Config_Calendar(struct RTC_Config_t *pRTCConfig)
{
	uint32_t time_value = 0;
	uint32_t date_value = 0;

	//1. Configure and Enable the RTC Clock
	//1.a Enable Write Access to the RTC Registers by enabling DBP bit in PWR_CR Register
	*pRCCPeriClkReg |= (1 << RCC_APB1ENR_PWR);
	*pPWR_CR |= (1 << PWR_CR_DBP);

	stm32_rtc->RTC_WPR = 0xCA;
	stm32_rtc->RTC_WPR = 0x53;

	//1.a Configure the RTC Clock Source in in RCC_BDCR

	*pRCC_BDCR &= ~(3 << RCC_BDCR_RTCSEL);
	*pRCC_BDCR |= (pRTCConfig->RTCClockSource << RCC_BDCR_RTCSEL);

	//1.b If the RTC Clock Source is HSE, Turn On the HSE Oscillator
	//Also, configure the HSE PreScaler to generate 1 MHz RTCCLK
	if(pRTCConfig->RTCClockSource == RTC_CLK_SRC_HSE)
	{
		//Turning on the HSE Oscillator
		*pRCC_CR |= (1 << HSE_ON_BITPOS);					//Turning on the HSE Oscillator
		while(!(*pRCC_CR & (1 << HSE_CHECK_BITPOS)));		//Checking whether HSE Oscillator is turned on and is stable

		//Configure the HSE PreScaler to generate 1 MHz RTCCLK
		*pRCC_CFGR &= ~(31 << RCC_CFGR_RTCPRE);
		*pRCC_CFGR |= (pRTCConfig->RTCClockHSEPrescaler << RCC_CFGR_RTCPRE);
	}

	//1.c Enable the RTC Clock in RCC_BDCR
	*pRCC_BDCR |= (1 << RCC_BDCR_RTCEN);

	//2. Configure the RTC Calendar Parameters

	//2.b Enable the Initialization Mode
	stm32_rtc->RTC_ISR |= (1 << RTC_ISR_INIT);
	while(!((stm32_rtc->RTC_ISR >> RTC_ISR_INITF) & 0x1));

	//2.c Configure the PreScaler Values in RTC_PRER Register
	stm32_rtc->RTC_PRER &= ~(0x7FFF << RTC_PRER_PREDIV_S);
	stm32_rtc->RTC_PRER |= (pRTCConfig->RTCClockPrescalerS << RTC_PRER_PREDIV_S);

	stm32_rtc->RTC_PRER &= ~(0x7F << RTC_PRER_PREDIV_A);
	stm32_rtc->RTC_PRER |= (pRTCConfig->RTCClockPrescalerA << RTC_PRER_PREDIV_A);

	//2.d Configure the Time Parameters in RTC_TR Register
	time_value = (pRTCConfig->RTCClockSecondsUnits << RTC_TR_SU) | (pRTCConfig->RTCClockSecondsTens << RTC_TR_ST) | (pRTCConfig->RTCClockMinutesUnits << RTC_TR_MNU) | (pRTCConfig->RTCClockMinutesTens << RTC_TR_MNT) | (pRTCConfig->RTCClockHoursUnits << RTC_TR_HU) |  (pRTCConfig->RTCClockHoursTens << RTC_TR_HT) | (pRTCConfig->RTCClockAMPM << RTC_TR_PM);
	stm32_rtc->RTC_TR = time_value;

	//2.e Configure the Date Parameters in RTC_DR Register
	date_value = (pRTCConfig->RTCClockDateUnits << RTC_DR_DU) | (pRTCConfig->RTCClockDateTens << RTC_DR_DT) | (pRTCConfig->RTCClockMonthUnits << RTC_DR_MU) | (pRTCConfig->RTCClockMonthTens << RTC_DR_MT) | (pRTCConfig->RTCClockYearUnits << RTC_DR_YU) | (pRTCConfig->RTCClockYearTens << RTC_DR_YT) | (pRTCConfig->RTCClockDayOfWeek << RTC_DR_WDU);
	stm32_rtc->RTC_DR = date_value;

	//2.f Exit the Initialization Mode
	stm32_rtc->RTC_ISR &= ~(1 << RTC_ISR_INIT);

	return;
}

void RTC_Read_Calendar(struct Date *date, struct Time *time)
{
	uint32_t time_value, date_value;

	//1. Wait for the RSF bit to be set in RTC_ISR register
	while(!((stm32_rtc->RTC_ISR >> RTC_ISR_RSF) & 0x1));

	//2. Read the current time value
	time_value = stm32_rtc->RTC_TR;

	//3. Read the current date value
	date_value = stm32_rtc->RTC_DR;

	//4. Format the time and date values
	//4.a Format the time values
	time->seconds = ((time_value >> RTC_TR_ST) & 0x7)*10 + ((time_value >> RTC_TR_SU) & 0xF);
	time->minutes = ((time_value >> RTC_TR_MNT) & 0x7)*10 + ((time_value >> RTC_TR_MNU) & 0xF);
	time->hours = ((time_value >> RTC_TR_HT) & 0x3)*10 + ((time_value >> RTC_TR_HU) & 0xF);
	time->timeformat = ((time_value >> RTC_TR_PM) & 0x1);

	//4.b Format the date values
	date->date = ((date_value >> RTC_DR_DT) & 0x3)*10 + ((date_value >> RTC_DR_DU) & 0xF);
	date->month = ((date_value >> RTC_DR_MT) & 0x1)*10 + ((date_value >> RTC_DR_MU) & 0xF);
	date->year = ((date_value >> RTC_DR_YT) & 0xF)*10 + ((date_value >> RTC_DR_YU) & 0xF);
	date->dayofweek = ((date_value >> RTC_DR_WDU) & 0x7);

	return;
}

void RTC_Display_Calendar_LCD(struct Date *date, struct Time *time)
{
	static uint32_t count = 0;
	struct Date saved_date;
	struct Time saved_time;
	char dow[4];
	char mon[4];

	//Ensure that printf is redirected to LCD is "syscalls.c" file
	if(count == 0)
	{
		//Print the Time Information
		lcd_pcf8574_clear_screen();
		delay_us(3000);
		lcd_pcf8574_return_home();
		delay_us(3000);
		printf("    %02d:%02d:%02d",time->hours,time->minutes,time->seconds);

		//Saving the time information
		memset(&saved_time,0,sizeof(struct Time));
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
		memset(&saved_date,0,sizeof(struct Date));
		saved_date.date = date->date;
		saved_date.month = date->month;
		saved_date.year = date->year;
		saved_date.dayofweek = date->dayofweek;

		//Update count value
		count++;
	}
	else
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

void RTC_Config_Alarm(struct RTC_AlarmConfig_t *pRTCAlarmConfig)
{
	uint32_t alarm_value = 0;

	//1a. Configure the ALARM Parameters in the appropriate ALARM Register
	//1b. Enable the ALARM Interrupt in the RTC_CR Register
	//1c. Enable the ALARM in RTC_CR Register
	alarm_value = (pRTCAlarmConfig->RTCAlarmSecondsUnits << RTC_ALRMR_SU) | (pRTCAlarmConfig->RTCAlarmSecondsTens << RTC_ALRMR_ST) | (pRTCAlarmConfig->RTCAlarmConsiderSeconds << RTC_ALRMR_MSK1) | (pRTCAlarmConfig->RTCAlarmMinutesUnits << RTC_ALRMR_MNU) | (pRTCAlarmConfig->RTCAlarmMinutesTens << RTC_ALRMR_MNT) | (pRTCAlarmConfig->RTCAlarmConsiderMinutes << RTC_ALRMR_MSK2) | (pRTCAlarmConfig->RTCAlarmHoursUnits << RTC_ALRMR_HU) |  (pRTCAlarmConfig->RTCAlarmHoursTens << RTC_ALRMR_HT) | (pRTCAlarmConfig->RTCAlarmAMPM << RTC_ALRMR_PM) | (pRTCAlarmConfig->RTCAlarmConsiderHours << RTC_ALRMR_MSK3) | (pRTCAlarmConfig->RTCAlarmDateUnits << RTC_ALRMR_DU) | (pRTCAlarmConfig->RTCAlarmDateTens << RTC_ALRMR_DT) | (pRTCAlarmConfig->RTCAlarmWeekDaySelection << RTC_ALRMR_WDSEL) | (pRTCAlarmConfig->RTCAlarmConsiderDate << RTC_ALRMR_MSK4);

	if(pRTCAlarmConfig->RTCAlarmSelection == RTC_ALARM_A)
	{
		stm32_rtc->RTC_ALRMAR = alarm_value;
		stm32_rtc->RTC_CR |= (1 << RTC_CR_ALRAIE);
		stm32_rtc->RTC_CR |= (1 << RTC_CR_ALRAE);
	}
	else if(pRTCAlarmConfig->RTCAlarmSelection == RTC_ALARM_B)
	{
		stm32_rtc->RTC_ALRMBR = alarm_value;
		stm32_rtc->RTC_CR |= (1 << RTC_CR_ALRBIE);
		stm32_rtc->RTC_CR |= (1 << RTC_CR_ALRBE);
	}

	return;
}

void RTC_Config_Alarm_Interrupt(void)
{
	uint32_t *pEXTI_IMR	= (uint32_t *) EXTI_IMR_ADDR;
	uint32_t *pEXTI_RTSR = (uint32_t *) EXTI_RTSR_ADDR;

	// 1. Configuring the EXTI Controller (External Interrupt Controller)
	*pEXTI_IMR |= (1 << RTC_ALARM_INTERRUPT_EXTI_PIN);  	// Setting the Interrupt Mask Register
	*pEXTI_RTSR |= (1 << RTC_ALARM_INTERRUPT_EXTI_PIN); 	// Setting the Rising Trigger Set Register

	// 2. // Enabling the interrupt in NVIC
	NVIC_EnableIRQ(RTC_ALARM_INTERRUPT_IRQ_NO);				// Enabling the interrupt

	return;
}

void RTC_Alarm_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	//1. Calling the RTC Alarm Interrupt Callback function
	RTC_Alarm_Interrupt_Callback();

	//2. Clearing the Interrupt
	*pEXTI_PR |= (1 << RTC_ALARM_INTERRUPT_EXTI_PIN);		// Clearing the EXTI_PR Register

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
