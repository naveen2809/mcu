/*
 * external_interrupt_config.h
 *
 *  Created on: 19-Dec-2023
 *      Author: naveen
 */

#ifndef INC_EXTERNAL_INTERRUPT_CONFIG_H_
#define INC_EXTERNAL_INTERRUPT_CONFIG_H_

#define INTERRUPT_PORT					GPIOA
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

void configure_external_interrupt(void);

#endif /* INC_EXTERNAL_INTERRUPT_CONFIG_H_ */
