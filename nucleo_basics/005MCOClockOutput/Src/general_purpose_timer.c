/*
 * general_purpose_timer.c
 *
 *  Created on: 02-Dec-2023
 *      Author: naveen
 */

#include "general_purpose_timer.h"

void General_Purpose_Timer_Init(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle)
{

	//1. Configure the PreScaler Value

	pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_PSC = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_PreScalerValue;

	//2. Configure the Auto Reload Value

	pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_ARR = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_AutoReloadValue;


	//3. Channel 1 Configuration

	if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_1_Status == TIMER_CHANNEL_ENABLED)
	{

		pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCER |= (0x1 << TIMx_CCER_CC1E);

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_1_Mode == TIMER_CHANNEL_MODE_OUTPUT_TOGGLE)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x3 << TIMx_CCMR1_CC1S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR1 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_1_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x7 << TIMx_CCMR1_OC1M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 |= (0x3 << TIMx_CCMR1_OC1M);
		}

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_1_Mode == TIMER_CHANNEL_MODE_OUTPUT_PWM1)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x3 << TIMx_CCMR1_CC1S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR1 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_1_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x7 << TIMx_CCMR1_OC1M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 |= (0x6 << TIMx_CCMR1_OC1M);
		}

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_1_Mode == TIMER_CHANNEL_MODE_OUTPUT_PWM2)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x3 << TIMx_CCMR1_CC1S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR1 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_1_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x7 << TIMx_CCMR1_OC1M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 |= (0x7 << TIMx_CCMR1_OC1M);
		}
	}

	//4. Channel 2 Configuration

	if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_2_Status == TIMER_CHANNEL_ENABLED)
	{

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_2_Mode == TIMER_CHANNEL_MODE_OUTPUT_TOGGLE)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x3 << TIMx_CCMR1_CC2S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR2 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_2_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x7 << TIMx_CCMR1_OC2M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 |= (0x3 << TIMx_CCMR1_OC2M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCER |= (0x1 << TIMx_CCER_CC2E);
		}

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_2_Mode == TIMER_CHANNEL_MODE_OUTPUT_PWM1)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x3 << TIMx_CCMR1_CC2S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR2 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_2_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x7 << TIMx_CCMR1_OC2M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 |= (0x6 << TIMx_CCMR1_OC2M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCER |= (0x1 << TIMx_CCER_CC2E);
		}

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_2_Mode == TIMER_CHANNEL_MODE_OUTPUT_PWM2)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x3 << TIMx_CCMR1_CC2S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR2 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_2_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x7 << TIMx_CCMR1_OC2M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 |= (0x7 << TIMx_CCMR1_OC2M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCER |= (0x1 << TIMx_CCER_CC2E);
		}

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_2_Mode == TIMER_CHANNEL_MODE_INPUT_CAPTURE)
		{
			//1. Setting the CC1S field to 01
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x3 << TIMx_CCMR1_CC2S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 |= (0x1 << TIMx_CCMR1_CC2S);

			//2. Configuring the input filter settings
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0xF << TIMx_CCMR1_IC2F);
			//pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 |= (0x5 << TIMx_CCMR1_IC2F);

			//3. Program the CC1P and CC1NP bits to 00
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCER &= ~(0x1 << TIMx_CCER_CC2P);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCER &= ~(0x1 << TIMx_CCER_CC2NP);

			//4. Programming the input pre-scaler value to 00 (no pre-scaler)
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR1 &= ~(0x3 << TIMx_CCMR1_IC2PSC);

			//5. Enable the input capture interrupt
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_DIER |= (0x1 << TIMx_DIER_CC2IE);

			//6. Turning on the channel
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCER |= (0x1 << TIMx_CCER_CC2E);
		}

	}

	//5. Channel 3 Configuration

	if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_3_Status == TIMER_CHANNEL_ENABLED)
	{
		pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCER |= (0x1 << TIMx_CCER_CC3E);

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_3_Mode == TIMER_CHANNEL_MODE_OUTPUT_TOGGLE)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x3 << TIMx_CCMR2_CC3S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR3 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_3_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x7 << TIMx_CCMR2_OC3M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 |= (0x3 << TIMx_CCMR2_OC3M);
		}

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_3_Mode == TIMER_CHANNEL_MODE_OUTPUT_PWM1)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x3 << TIMx_CCMR2_CC3S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR3 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_3_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x7 << TIMx_CCMR2_OC3M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 |= (0x6 << TIMx_CCMR2_OC3M);
		}

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_3_Mode == TIMER_CHANNEL_MODE_OUTPUT_PWM2)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x3 << TIMx_CCMR2_CC3S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR3 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_3_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x7 << TIMx_CCMR2_OC3M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 |= (0x7 << TIMx_CCMR2_OC3M);
		}
	}

	//6. Channel 4 Configuration

	if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_4_Status == TIMER_CHANNEL_ENABLED)
	{
		pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCER |= (0x1 << TIMx_CCER_CC4E);

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_4_Mode == TIMER_CHANNEL_MODE_OUTPUT_TOGGLE)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x3 << TIMx_CCMR2_CC4S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR4 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_4_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x7 << TIMx_CCMR2_OC4M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 |= (0x3 << TIMx_CCMR2_OC4M);
		}

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_4_Mode == TIMER_CHANNEL_MODE_OUTPUT_PWM1)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x3 << TIMx_CCMR2_CC4S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR4 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_4_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x7 << TIMx_CCMR2_OC4M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 |= (0x6 << TIMx_CCMR2_OC4M);
		}

		if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_4_Mode == TIMER_CHANNEL_MODE_OUTPUT_PWM2)
		{
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x3 << TIMx_CCMR2_CC4S);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCR4 = pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_Channel_4_OutputCompareRegisterValue;
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 &= ~(0x7 << TIMx_CCMR2_OC4M);
			pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CCMR2 |= (0x7 << TIMx_CCMR2_OC4M);
		}
	}

	/*
	//4. Enable/Disable the Update Event Interrupt

	if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_UpdateInterruptConfig == TIMER_UPDATE_INTERRUPT_ENABLE)
	{
		pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_DIER |= (1 << TIMx_DIER_UIE);
	}
	else if(pGeneral_Purpose_Timer_Handle->General_Purpose_Timer_Config.Timer_UpdateInterruptConfig == TIMER_UPDATE_INTERRUPT_DISABLE)
	{
		pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_DIER &= ~(1 << TIMx_DIER_UIE);
	}
	*/

}

void General_Purpose_Timer_PeripheralEnable(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle)
{
	pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CR1 |= (1 << TIMx_CR1_CEN);
}

void General_Purpose_Timer_PeripheralDisable(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle)
{
	pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CR1 &= ~(1 << TIMx_CR1_CEN);
}

void General_Purpose_Timer_EnableUpdateEventInterrupt(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle)
{
	pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_DIER |= (1 << TIMx_DIER_UIE);
}

void General_Purpose_Timer_DisableUpdateEventInterrupt(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle)
{
	pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_DIER &= ~(1 << TIMx_DIER_UIE);
}

void General_Purpose_Timer_ClearUpdateEventFlag(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle)
{
	pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_SR &= ~(1 << TIMx_SR_UIF);
}

void General_Purpose_Timer_SetARPEFlag(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle)
{
	pGeneral_Purpose_Timer_Handle->pGeneral_Purpose_Timer->TIMx_CR1 |= (1 << TIMx_CR1_ARPE);
}

/*
void General_Purpose_Timer_IRQHandling(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle)
{
	Basic_Timer_Interrupt_Callback(pBasic_Timer_Handle);
}
*/

