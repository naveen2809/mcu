################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./Core/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./Core/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/port.o: /home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/port.c Core/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/include" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/Config" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/OS" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER" -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Common-2f-ThirdParty-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM4F

clean-Core-2f-Common-2f-ThirdParty-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM4F:
	-$(RM) ./Core/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/port.d ./Core/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/port.o ./Core/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/port.su

.PHONY: clean-Core-2f-Common-2f-ThirdParty-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM4F

