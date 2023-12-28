################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/temp_on_lcd_over_wireless_tx.c 

OBJS += \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/temp_on_lcd_over_wireless_tx.o 

C_DEPS += \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/temp_on_lcd_over_wireless_tx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -I"/home/naveen/Code/mcu/nucleo_basics/003I2CTesting/Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/temp_on_lcd_over_wireless_tx.d ./Src/temp_on_lcd_over_wireless_tx.o ./Src/temp_on_lcd_over_wireless_tx.su

.PHONY: clean-Src

