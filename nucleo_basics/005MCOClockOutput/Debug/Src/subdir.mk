################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/general_purpose_timer.c \
../Src/general_purpose_timer_toggle_36MHz.c \
../Src/gpio_driver.c \
../Src/stm32f4xx.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/general_purpose_timer.o \
./Src/general_purpose_timer_toggle_36MHz.o \
./Src/gpio_driver.o \
./Src/stm32f4xx.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/general_purpose_timer.d \
./Src/general_purpose_timer_toggle_36MHz.d \
./Src/gpio_driver.d \
./Src/stm32f4xx.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/general_purpose_timer.d ./Src/general_purpose_timer.o ./Src/general_purpose_timer.su ./Src/general_purpose_timer_toggle_36MHz.d ./Src/general_purpose_timer_toggle_36MHz.o ./Src/general_purpose_timer_toggle_36MHz.su ./Src/gpio_driver.d ./Src/gpio_driver.o ./Src/gpio_driver.su ./Src/stm32f4xx.d ./Src/stm32f4xx.o ./Src/stm32f4xx.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

