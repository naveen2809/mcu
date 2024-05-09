#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "FreeRTOS.h"
#include "task.h"

#define LED_GPIO_PORT       GPIOD
#define GPIO_GREEN_LED      GPIO_PIN_12
#define GPIO_ORANGE_LED     GPIO_PIN_13
#define GPIO_RED_LED        GPIO_PIN_14
#define GPIO_BLUE_LED       GPIO_PIN_15  

static void task_green_led(void *params);
static void task_orange_led(void *params);
static void task_red_led(void *params);
static void task_blue_led(void *params);

static void delay(uint32_t count);

int main(void)
{
    BaseType_t status;
    TaskHandle_t task_handle_green_led;
    TaskHandle_t task_handle_orange_led;
    TaskHandle_t task_handle_red_led;
    TaskHandle_t task_handle_blue_led;

    //GPIO Configuration
    EnablePeriClk(LED_GPIO_PORT);
    GPIOSetMode(LED_GPIO_PORT,GPIO_GREEN_LED,GPIO_MODE_OUTPUT);
    GPIOSetMode(LED_GPIO_PORT,GPIO_ORANGE_LED,GPIO_MODE_OUTPUT);
    GPIOSetMode(LED_GPIO_PORT,GPIO_RED_LED,GPIO_MODE_OUTPUT);
    GPIOSetMode(LED_GPIO_PORT,GPIO_BLUE_LED,GPIO_MODE_OUTPUT);

    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();
    
    status = xTaskCreate(task_green_led,"green_led_task",1024,NULL,1,&task_handle_green_led);
    configASSERT(status == pdPASS);

    status = xTaskCreate(task_orange_led,"orange_led_task",1024,NULL,1,&task_handle_orange_led);
    configASSERT(status == pdPASS);

    status = xTaskCreate(task_red_led,"red_led_task",1024,NULL,1,&task_handle_red_led);
    configASSERT(status == pdPASS);

    status = xTaskCreate(task_blue_led,"blue_led_task",1024,NULL,1,&task_handle_blue_led);
    configASSERT(status == pdPASS);

    


    vTaskStartScheduler();

    while(1)
    {

    }

    return 0;
}

static void task_green_led(void *params)
{
    while(1)
    {
        GPIOTogglePin(LED_GPIO_PORT,GPIO_GREEN_LED);
        delay(250000);
    }
}

static void task_orange_led(void *params)
{
    while(1)
    {
        GPIOTogglePin(LED_GPIO_PORT,GPIO_ORANGE_LED);
        delay(500000);
    }
}

static void task_red_led(void *params)
{
    while(1)
    {
        GPIOTogglePin(LED_GPIO_PORT,GPIO_RED_LED);
        delay(750000);
    }
}

static void task_blue_led(void *params)
{
    while(1)
    {
        GPIOTogglePin(LED_GPIO_PORT,GPIO_BLUE_LED);
        delay(1000000);
    }
}

static void delay(uint32_t count)
{
    uint32_t i;
    for(i=0;i<count;i++)
    {

    }
}