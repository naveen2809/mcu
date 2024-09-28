#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "usart_driver.h"
#include "dma_driver.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define SCB_AIRCR_VECTKEY_Msk (0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)
#define SCB_AIRCR_PRIGROUP_Msk (7UL << SCB_AIRCR_PRIGROUP_Pos)
#define SCB_AIRCR_VECTKEY_Pos 16U
#define SCB_AIRCR_PRIGROUP_Pos 8U

#define UART_TX_PIN		GPIO_PIN_2
#define UART_RX_PIN		GPIO_PIN_3

#define USART2_IRQ_NO	38

struct USART_Handle_t Test_USART;

static void task_uart_interrupt(void *params);
static void NVIC_SetPriorityGrouping(void);
static void configure_uart(void);

static TaskHandle_t task_uart_interrupt_handle;
static SemaphoreHandle_t xSemaphore;

char message[]="Hello World!!!\r\n";

int main(void)
{

	BaseType_t status;

	NVIC_SetPriorityGrouping();

	configure_uart();
	NVIC_SetPriority(NVIC_PRIO_REG_BASE,USART2_IRQ_NO,100);
	NVIC_EnableIRQ(USART2_IRQ_NO);		// Enabling the interrupt

	traceSTART();

	status = xTaskCreate(task_uart_interrupt,"Task_UART_INTERRUPT",200,NULL,2,&task_uart_interrupt_handle);
  	configASSERT(status == pdPASS);

  	xSemaphore = xSemaphoreCreateBinary();
  	configASSERT(xSemaphore != NULL);

  	vTaskStartScheduler();

	/* Loop forever */
	while(1)
	{
		
	}

	return 0;
}

static void task_uart_interrupt(void *params)
{
	uint32_t len, count;

	while(1)
	{
		len = strlen(message);
		count = 0;

		//Clear and enable TC Interrupt at the beginning of message transmission
		Test_USART.pUSART->USART_SR &= ~(1 << 6);
		Test_USART.pUSART->USART_CR1 |= (1 << 6);

		xSemaphoreGive(xSemaphore);

		while(len > 0)
		{
			if(xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE)
			{
				Test_USART.pUSART->USART_DR = message[count];
			}

			len--;
			count++;
		}

		if(len == 0)
		{
			//Disable TC Interrupt at the end of message transmission
			Test_USART.pUSART->USART_CR1 &= ~(1 << 6);
		}
	}
}

void USART2_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	SEGGER_SYSVIEW_Print("Inside USART Interrupt Handler");

	//Clear the TC Interrupt
	Test_USART.pUSART->USART_SR &= ~(1 << 6);

	xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
}

static void NVIC_SetPriorityGrouping(void)
{

	uint32_t *pSCB_AIRCR = (uint32_t *) 0xE000ED0CUL;

	uint32_t value = *pSCB_AIRCR;

	value &= ~((uint32_t)(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk));

	value = value | ((uint32_t)0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | (0x3 << SCB_AIRCR_PRIGROUP_Pos);

	*pSCB_AIRCR = value;

}

static void configure_uart(void)
{
	//GPIO Pin Configuration
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA,UART_TX_PIN,GPIO_MODE_ALTFN);
	GPIOSetMode(GPIOA,UART_RX_PIN,GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,UART_TX_PIN,GPIO_ALTFN_7);
	GPIOSetAltFn(GPIOA,UART_RX_PIN,GPIO_ALTFN_7);
	GPIOSetOutputType(GPIOA,UART_TX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputType(GPIOA,UART_RX_PIN,GPIO_OPTYPE_PP);
	GPIOSetOutputSpeed(GPIOA,UART_TX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetOutputSpeed(GPIOA,UART_RX_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(GPIOA,UART_TX_PIN,GPIO_PULL_UP);
	GPIOSetPullUpDownConfig(GPIOA,UART_RX_PIN,GPIO_PULL_UP);

	//USART Configuration
	Test_USART.pUSART = (struct USART_RegDef_t *) USART2;
	Test_USART.USART_Config.USART_Mode = USART_MODE_TX_RX;
	Test_USART.USART_Config.USART_DataLength = USART_DATA_LEN_8_BITS;
	Test_USART.USART_Config.USART_StopBits = USART_STOP_BITS_1;
	Test_USART.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	Test_USART.USART_Config.USART_BaudRate = USART_SB_RATE_9600;
	Test_USART.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CNTRL_NONE;

	EnablePeriClk(USART2);
	USART_Init(&Test_USART);
	USART_PeripheralEnable(&Test_USART);
}
