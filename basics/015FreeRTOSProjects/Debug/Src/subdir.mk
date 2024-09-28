################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main_uart_interrupt.c \
../Src/syscalls.c 

OBJS += \
./Src/main_uart_interrupt.o \
./Src/syscalls.o 

C_DEPS += \
./Src/main_uart_interrupt.d \
./Src/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/Config" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/Sample/FreeRTOS" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/SEGGER" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/Drivers/Inc" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/FreeRTOS/include" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main_uart_interrupt.d ./Src/main_uart_interrupt.o ./Src/main_uart_interrupt.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su

.PHONY: clean-Src

