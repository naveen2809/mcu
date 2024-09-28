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

static void NVIC_SetPriorityGrouping(void);
static void default_task(void *params);
static void GreenLedCallbackFunction(TimerHandle_t xTimer);
static void OrangeLedCallbackFunction(TimerHandle_t xTimer);
static void RedLedCallbackFunction(TimerHandle_t xTimer);
static void BlueLedCallbackFunction(TimerHandle_t xTimer);

static TaskHandle_t default_task_handle;

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

	//Creating the Task where SW Timers are Started
	status = xTaskCreate(default_task,"Default_Task",200,NULL,8,&default_task_handle);

  	configASSERT(status == pdPASS);

	//Creating the SW Timers
	green_led_timer = xTimerCreate("green led timer",250,pdTRUE,NULL,GreenLedCallbackFunction);
	configASSERT(green_led_timer != NULL);

	orange_led_timer = xTimerCreate("orange led timer",500,pdTRUE,NULL,OrangeLedCallbackFunction);
	configASSERT(orange_led_timer != NULL);

	red_led_timer = xTimerCreate("red led timer",750,pdTRUE,NULL,RedLedCallbackFunction);
	configASSERT(red_led_timer != NULL);

	blue_led_timer = xTimerCreate("blue led timer",1000,pdTRUE,NULL,BlueLedCallbackFunction);
	configASSERT(blue_led_timer != NULL);		
	
	//Starting the SW Timers
	xTimerStart(green_led_timer, 0);
	configASSERT(status == pdPASS);

	xTimerStart(orange_led_timer, 0);
	configASSERT(status == pdPASS);

	xTimerStart(red_led_timer, 0);
	configASSERT(status == pdPASS);

	xTimerStart(blue_led_timer, 0);
	configASSERT(status == pdPASS);	
	
	//Starting the Scheduler
  	vTaskStartScheduler();

	/* Loop forever */
	while(1)
	{
		
	}

	return 0;
}

static void default_task(void *params)
{
	while(1)
	{
		vTaskDelay(500);
	}
}

//Callback functions for the default timers
static void GreenLedCallbackFunction(TimerHandle_t xTimer)
{
	GPIOTogglePin(GPIOD, GPIO_PIN_12);
}

static void OrangeLedCallbackFunction(TimerHandle_t xTimer)
{
	GPIOTogglePin(GPIOD, GPIO_PIN_13);
}

static void RedLedCallbackFunction(TimerHandle_t xTimer)
{
	GPIOTogglePin(GPIOD, GPIO_PIN_14);
}

static void BlueLedCallbackFunction(TimerHandle_t xTimer)
{
	GPIOTogglePin(GPIOD, GPIO_PIN_15);
}

static void NVIC_SetPriorityGrouping(void)
{

	uint32_t *pSCB_AIRCR = (uint32_t *) 0xE000ED0CUL;

	uint32_t value = *pSCB_AIRCR;

	value &= ~((uint32_t)(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk));

	value = value | ((uint32_t)0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | (0x3 << SCB_AIRCR_PRIGROUP_Pos);

	*pSCB_AIRCR = value;

}
