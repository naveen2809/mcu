#include <stdint.h>
#include <stdio.h>
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

#define UART_TX_PIN						GPIO_PIN_2
#define UART_RX_PIN						GPIO_PIN_3

#define DMA1_STREAM6_IRQ_NO				17

struct USART_Handle_t Test_USART;
struct DMA_Handle_t Test_DMA;

static void task_uart_dma(void *params);
static void NVIC_SetPriorityGrouping(void);
static void configure_uart(void);
static void configure_dma(void);

static TaskHandle_t task_uart_dma_handle;
static SemaphoreHandle_t xSemaphore;

char message[]="Hello World!!!\r\n";

int main(void)
{

	BaseType_t status;

	NVIC_SetPriorityGrouping();

	configure_uart();
	configure_dma();

	traceSTART();

	status = xTaskCreate(task_uart_dma,"Task_UART_DMA",200,NULL,2,&task_uart_dma_handle);
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

static void task_uart_dma(void *params)
{
	//Triggering the DMA for the first time
	Test_USART.pUSART->USART_CR3 |= (1 << USART_CR3_DMAT);

	while(1)
	{
		if(xSemaphoreTake(xSemaphore,portMAX_DELAY) == pdTRUE)
		{
			//Configure the number of bytes to be transferred
			Test_DMA.pDMA_Stream->DMA_NDTR = Test_DMA.DMA_Config.DMA_Num_Bytes;

			//Enable DMA
			DMA_PeripheralEnable(&Test_DMA);

			Test_USART.pUSART->USART_CR3 |= (1 << USART_CR3_DMAT);
		}
	}
}

void DMA1_Stream6_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	//Clear the configuration in UART CR register
	Test_USART.pUSART->USART_CR3 &= ~(1 << USART_CR3_DMAT);

	//Clear DMA TCIE Interrupt Flag
	Test_DMA.pDMA_Cont->DMA_HIFCR |= (1 << 21);

	SEGGER_SYSVIEW_Print("Inside DMA Transfer Complete ISR");

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

static void configure_dma(void)
{
	Test_DMA.pDMA_Cont = (struct DMA_Controller_RegDef_t *) DMA1;
	Test_DMA.pDMA_Stream = (struct DMA_Stream_RegDef_t *) DMA1_STREAM_6;
	Test_DMA.DMA_Config.DMA_Channel = DMA_CHANNEL_4;
	Test_DMA.DMA_Config.DMA_Peripheral_Address = (uint32_t) &(Test_USART.pUSART->USART_DR);
	Test_DMA.DMA_Config.DMA_M0_Address = (uint32_t) &message;
	Test_DMA.DMA_Config.DMA_Memory_Data_Size = DMA_DATA_SIZE_BYTE;
	Test_DMA.DMA_Config.DMA_Peripheral_Data_Size = DMA_DATA_SIZE_BYTE;
	Test_DMA.DMA_Config.DMA_Memory_Address_Increment = DMA_ADDR_INCREMENT;
	Test_DMA.DMA_Config.DMA_Peripheral_Address_Increment = DMA_ADDR_FIXED;
	Test_DMA.DMA_Config.DMA_Data_Transfer_Direction = DMA_DIRECTION_M2P;
	Test_DMA.DMA_Config.DMA_Stream_Priority_Level = DMA_PRIORITY_LEVEL_LOW;
	Test_DMA.DMA_Config.DMA_Num_Bytes = sizeof(message);
	Test_DMA.DMA_Config.DMA_TCIE_Status = DMA_TCIE_ENABLE;
	Test_DMA.DMA_Config.DMA_HTIE_Status = DMA_HTIE_DISABLE;

	EnablePeriClk(DMA1);
	DMA_Config(&Test_DMA);
	DMA_PeripheralEnable(&Test_DMA);

	// Enable the DMA TC Interrupt
	NVIC_SetPriority(NVIC_PRIO_REG_BASE,DMA1_STREAM6_IRQ_NO,100);
	NVIC_EnableIRQ(DMA1_STREAM6_IRQ_NO);		// Enabling the interrupt
}
