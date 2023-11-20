################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/Src/gpio_driver.c \
../Driver/Src/stm32f4xx.c 

OBJS += \
./Driver/Src/gpio_driver.o \
./Driver/Src/stm32f4xx.o 

C_DEPS += \
./Driver/Src/gpio_driver.d \
./Driver/Src/stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Driver/Src/%.o Driver/Src/%.su: ../Driver/Src/%.c Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -I"/home/naveen/Code/mcu/nucleo_basics/004LEDBlinking/Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Driver-2f-Src

clean-Driver-2f-Src:
	-$(RM) ./Driver/Src/gpio_driver.d ./Driver/Src/gpio_driver.o ./Driver/Src/gpio_driver.su ./Driver/Src/stm32f4xx.d ./Driver/Src/stm32f4xx.o ./Driver/Src/stm32f4xx.su

.PHONY: clean-Driver-2f-Src

