################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c 

OBJS += \
./Common/ThirdParty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o 

C_DEPS += \
./Common/ThirdParty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d 


# Each subdirectory must supply rules for building sources it contributes
Common/ThirdParty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o: /home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c Common/ThirdParty/SEGGER/SEGGER/Syscalls/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/SEGGER" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/OS" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/SEGGER/Config" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/include" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/Common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"/home/naveen/Code/mcu/basics/020FreeRTOS/FreeRTOS_Workspace/006MutexBareMetal/Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Common-2f-ThirdParty-2f-SEGGER-2f-SEGGER-2f-Syscalls

clean-Common-2f-ThirdParty-2f-SEGGER-2f-SEGGER-2f-Syscalls:
	-$(RM) ./Common/ThirdParty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.d ./Common/ThirdParty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.o ./Common/ThirdParty/SEGGER/SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.su

.PHONY: clean-Common-2f-ThirdParty-2f-SEGGER-2f-SEGGER-2f-Syscalls

