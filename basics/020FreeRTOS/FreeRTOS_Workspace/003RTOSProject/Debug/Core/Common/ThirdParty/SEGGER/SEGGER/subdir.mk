################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.c \
/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.c \
/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.c 

S_UPPER_SRCS += \
/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.o \
./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.o \
./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.o 

S_UPPER_DEPS += \
./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.d \
./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.d \
./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.o: /home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.c Core/Common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/include" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/Config" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/OS" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o: /home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S Core/Common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/Config" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.o: /home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.c Core/Common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/include" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/Config" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/OS" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.o: /home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.c Core/Common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/include" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/Config" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/OS" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Common-2f-ThirdParty-2f-SEGGER-2f-SEGGER

clean-Core-2f-Common-2f-ThirdParty-2f-SEGGER-2f-SEGGER:
	-$(RM) ./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.d ./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.o ./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.su ./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d ./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o ./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.d ./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.o ./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.su ./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.d ./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.o ./Core/Common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.su

.PHONY: clean-Core-2f-Common-2f-ThirdParty-2f-SEGGER-2f-SEGGER

