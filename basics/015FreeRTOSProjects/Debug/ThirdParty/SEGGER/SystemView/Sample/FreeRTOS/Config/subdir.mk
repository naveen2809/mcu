################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/SEGGER/SystemView/Sample/FreeRTOS/Config/SEGGER_SYSVIEW_Config_FreeRTOS.c 

OBJS += \
./ThirdParty/SEGGER/SystemView/Sample/FreeRTOS/Config/SEGGER_SYSVIEW_Config_FreeRTOS.o 

C_DEPS += \
./ThirdParty/SEGGER/SystemView/Sample/FreeRTOS/Config/SEGGER_SYSVIEW_Config_FreeRTOS.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/SEGGER/SystemView/Sample/FreeRTOS/Config/%.o ThirdParty/SEGGER/SystemView/Sample/FreeRTOS/Config/%.su: ../ThirdParty/SEGGER/SystemView/Sample/FreeRTOS/Config/%.c ThirdParty/SEGGER/SystemView/Sample/FreeRTOS/Config/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/Config" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/Sample/FreeRTOS" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/SEGGER" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/Drivers/Inc" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/FreeRTOS/include" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-ThirdParty-2f-SEGGER-2f-SystemView-2f-Sample-2f-FreeRTOS-2f-Config

clean-ThirdParty-2f-SEGGER-2f-SystemView-2f-Sample-2f-FreeRTOS-2f-Config:
	-$(RM) ./ThirdParty/SEGGER/SystemView/Sample/FreeRTOS/Config/SEGGER_SYSVIEW_Config_FreeRTOS.d ./ThirdParty/SEGGER/SystemView/Sample/FreeRTOS/Config/SEGGER_SYSVIEW_Config_FreeRTOS.o ./ThirdParty/SEGGER/SystemView/Sample/FreeRTOS/Config/SEGGER_SYSVIEW_Config_FreeRTOS.su

.PHONY: clean-ThirdParty-2f-SEGGER-2f-SystemView-2f-Sample-2f-FreeRTOS-2f-Config

