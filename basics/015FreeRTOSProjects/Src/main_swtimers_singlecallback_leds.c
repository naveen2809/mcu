#include <stdint.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#define SCB_AIRCR_VECTKEY_Msk (0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)
#define SCB_AIRCR_PRIGROUP_Msk (7UL << SCB_AIRCR_PRIGROUP_Pos)
#define SCB_AIRCR_VECTKEY_Pos 16U
#define SCB_AIRCR_PRIGROUP_Pos 8U

#define GREEN_LED_TIMER_ID	1
#define ORANGE_LED_TIMER_ID	2
#define RED_LED_TIMER_ID	3
#define BLUE_LED_TIMER_ID	4

static void NVIC_SetPriorityGrouping(void);
static void LedCallbackFunction(TimerHandle_t xTimer);

static TimerHandle_t green_led_timer;
static TimerHandle_t orange_led_timer;
static TimerHandle_t red_led_timer;
static TimerHandle_t blue_led_timer;

int main(void)
{
	BaseType_t status;	
	
	NVIC_SetPriorityGrouping();

	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOD, GPIO_PIN_12, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_13, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_14, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_15, GPIO_MODE_OUTPUT);

	traceSTART();

	//Creating the SW Timers
	green_led_timer = xTimerCreate("green led timer",250,pdTRUE,(void *)GREEN_LED_TIMER_ID,LedCallbackFunction);
	configASSERT(green_led_timer != NULL);

	orange_led_timer = xTimerCreate("orange led timer",500,pdFALSE,(void *)ORANGE_LED_TIMER_ID,LedCallbackFunction);
	configASSERT(orange_led_timer != NULL);

	red_led_timer = xTimerCreate("red led timer",750,pdTRUE,(void *)RED_LED_TIMER_ID,LedCallbackFunction);
	configASSERT(red_led_timer != NULL);

	blue_led_timer = xTimerCreate("blue led timer",1000,pdFALSE,(void *)BLUE_LED_TIMER_ID,LedCallbackFunction);
	configASSERT(blue_led_timer != NULL);		
	
	//Starting the SW Timers
	status = xTimerStart(green_led_timer, 0);
	configASSERT(status == pdPASS);

	status = xTimerStart(orange_led_timer, 0);
	configASSERT(status == pdPASS);

	status = xTimerStart(red_led_timer, 0);
	configASSERT(status == pdPASS);

	status = xTimerStart(blue_led_timer, 0);
	configASSERT(status == pdPASS);	
	
	//Starting the Scheduler
  	vTaskStartScheduler();

	/* Loop forever */
	while(1)
	{
		
	}

	return 0;
}

//Callback functions for the default timers
static void LedCallbackFunction(TimerHandle_t xTimer)
{
	
	configASSERT(xTimer);

	if(((uint32_t) pvTimerGetTimerID( xTimer)) == GREEN_LED_TIMER_ID)
	{
		GPIOTogglePin(GPIOD, GPIO_PIN_12);
	}
	else if(((uint32_t) pvTimerGetTimerID( xTimer)) == ORANGE_LED_TIMER_ID)
	{
		GPIOTogglePin(GPIOD, GPIO_PIN_13);
	}
	else if(((uint32_t) pvTimerGetTimerID( xTimer)) == RED_LED_TIMER_ID)
	{
		GPIOTogglePin(GPIOD, GPIO_PIN_14);	
	}
	else if(((uint32_t) pvTimerGetTimerID( xTimer)) == BLUE_LED_TIMER_ID)
	{
		GPIOTogglePin(GPIOD, GPIO_PIN_15);
	}	
	
}

static void NVIC_SetPriorityGrouping(void)
{

	uint32_t *pSCB_AIRCR = (uint32_t *) 0xE000ED0CUL;

	uint32_t value = *pSCB_AIRCR;

	value &= ~((uint32_t)(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk));

	value = value | ((uint32_t)0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | (0x3 << SCB_AIRCR_PRIGROUP_Pos);

	*pSCB_AIRCR = value;

}
