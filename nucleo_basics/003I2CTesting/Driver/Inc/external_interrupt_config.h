/*
 * external_interrupt_config.h
 *
 *  Created on: 19-Dec-2023
 *      Author: naveen
 */

#ifndef INC_EXTERNAL_INTERRUPT_CONFIG_H_
#define INC_EXTERNAL_INTERRUPT_CONFIG_H_

#define INTERRUPT_PORT					GPIOE
#define INTERRUPT_PIN					GPIO_PIN_0
#define INTERRUPT_IRQ_NO				6
#define SYSCFG_EXTI_CR_BASE_ADDR		(0x40013800UL + 0x08UL)
#define EXTI_IMR_ADDR					(0x40013C00UL)
#define EXTI_PR_ADDR					(0x40013C00UL + 0x14UL)
#define EXTI_RTSR_ADDR					(0x40013C00UL + 0x08UL)
#define EXTI_FTSR_ADDR					(0x40013C00UL + 0x0CUL)
#define EXTI_RISING_TRIGGER_ENABLE		FALSE
#define EXTI_FALLING_TRIGGER_ENABLE		TRUE
#define	TRUE							1
#define FALSE							0
#define EXTI_RISING_TRIGGER				0
#define EXTI_FALLING_TRIGGER			1

void configure_external_interrupt(void);
void configure_external_gpio_interrupt(uint32_t *gpio_port,uint8_t gpio_pin,uint8_t trigger_choice,uint8_t irq_no);

#endif /* INC_EXTERNAL_INTERRUPT_CONFIG_H_ */
