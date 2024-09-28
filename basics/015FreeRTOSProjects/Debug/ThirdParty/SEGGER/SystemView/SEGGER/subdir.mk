################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT.c \
../ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT_printf.c \
../ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_SYSVIEW.c 

S_UPPER_SRCS += \
../ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT.o \
./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT_printf.o \
./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_SYSVIEW.o 

S_UPPER_DEPS += \
./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT.d \
./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT_printf.d \
./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/SEGGER/SystemView/SEGGER/%.o ThirdParty/SEGGER/SystemView/SEGGER/%.su: ../ThirdParty/SEGGER/SystemView/SEGGER/%.c ThirdParty/SEGGER/SystemView/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/Config" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/Sample/FreeRTOS" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/SEGGER" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/Drivers/Inc" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/FreeRTOS/include" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=softfp -mthumb -o "$@"
ThirdParty/SEGGER/SystemView/SEGGER/%.o: ../ThirdParty/SEGGER/SystemView/SEGGER/%.S ThirdParty/SEGGER/SystemView/SEGGER/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/Config" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=softfp -mthumb -o "$@" "$<"

clean: clean-ThirdParty-2f-SEGGER-2f-SystemView-2f-SEGGER

clean-ThirdParty-2f-SEGGER-2f-SystemView-2f-SEGGER:
	-$(RM) ./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT.d ./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT.o ./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT.su ./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT_ASM_ARMv7M.d ./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT_ASM_ARMv7M.o ./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT_printf.d ./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT_printf.o ./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_RTT_printf.su ./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_SYSVIEW.d ./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_SYSVIEW.o ./ThirdParty/SEGGER/SystemView/SEGGER/SEGGER_SYSVIEW.su

.PHONY: clean-ThirdParty-2f-SEGGER-2f-SystemView-2f-SEGGER

