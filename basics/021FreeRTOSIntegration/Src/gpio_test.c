#include <stdint.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"

#include "FreeRTOS.h"
#include "task.h"

#define SCB_AIRCR_VECTKEY_Msk (0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)
#define SCB_AIRCR_PRIGROUP_Msk (7UL << SCB_AIRCR_PRIGROUP_Pos)
#define SCB_AIRCR_VECTKEY_Pos 16U
#define SCB_AIRCR_PRIGROUP_Pos 8U

static void task_green_led(void *params);
static void task_orange_led(void *params);
static void task_red_led(void *params);
static void task_blue_led(void *params);

static void NVIC_SetPriorityGrouping(void);

static TaskHandle_t task_green_led_handle;
static TaskHandle_t task_orange_led_handle;
static TaskHandle_t task_red_led_handle;
static TaskHandle_t task_blue_led_handle;

int main(void)
{

	BaseType_t status;

	NVIC_SetPriorityGrouping();
	vInitPrioGroupValue();

	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOD, GPIO_PIN_12, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_13, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_14, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_15, GPIO_MODE_OUTPUT);

	status = xTaskCreate(task_green_led,"Task_Green_LED",200,NULL,2,&task_green_led_handle);

  	configASSERT(status == pdPASS);

  	status = xTaskCreate(task_orange_led,"Task_Orange_LED",200,NULL,2,&task_orange_led_handle);

  	configASSERT(status == pdPASS);

  	status = xTaskCreate(task_red_led,"Task_Red_LED",200,NULL,2,&task_red_led_handle);

  	configASSERT(status == pdPASS);

	status = xTaskCreate(task_blue_led,"Task_Blue_LED",200,NULL,2,&task_blue_led_handle);

  	configASSERT(status == pdPASS);

  	vTaskStartScheduler();

	/* Loop forever */
	while(1)
	{
		
	}

	return 0;
}

static void task_green_led(void *params)
{
	while(1)
	{
		GPIOTogglePin(GPIOD, GPIO_PIN_12);
		vTaskDelay(250);
	}
}

static void task_orange_led(void *params)
{
	while(1)
	{
		GPIOTogglePin(GPIOD, GPIO_PIN_13);
		vTaskDelay(500);
	}
}

static void task_red_led(void *params)
{
	while(1)
	{
		GPIOTogglePin(GPIOD, GPIO_PIN_14);
		vTaskDelay(750);
	}
}

static void task_blue_led(void *params)
{
	
	uint32_t task_resumed = 0;
	TickType_t os_tick_count;

	vTaskSuspend(task_green_led_handle);
	vTaskSuspend(task_orange_led_handle);
	vTaskSuspend(task_red_led_handle);

	while(1)
	{
		GPIOTogglePin(GPIOD, GPIO_PIN_15);
		vTaskDelay(1000);

		os_tick_count = xTaskGetTickCount();
		
		if(task_resumed == 0 && os_tick_count >= 60000)
		{

			vTaskResume(task_green_led_handle);
			vTaskResume(task_orange_led_handle);
			vTaskResume(task_red_led_handle);
			task_resumed = 1;
		}

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