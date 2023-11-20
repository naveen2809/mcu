################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/Src/gpio_driver.c \
../Driver/Src/i2c_driver.c \
../Driver/Src/spi_driver.c \
../Driver/Src/stm32f4xx.c \
../Driver/Src/usart_driver.c 

OBJS += \
./Driver/Src/gpio_driver.o \
./Driver/Src/i2c_driver.o \
./Driver/Src/spi_driver.o \
./Driver/Src/stm32f4xx.o \
./Driver/Src/usart_driver.o 

C_DEPS += \
./Driver/Src/gpio_driver.d \
./Driver/Src/i2c_driver.d \
./Driver/Src/spi_driver.d \
./Driver/Src/stm32f4xx.d \
./Driver/Src/usart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Driver/Src/%.o Driver/Src/%.su: ../Driver/Src/%.c Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -I"/home/naveen/Code/mcu/nucleo_basics/003I2CTesting/Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Driver-2f-Src

clean-Driver-2f-Src:
	-$(RM) ./Driver/Src/gpio_driver.d ./Driver/Src/gpio_driver.o ./Driver/Src/gpio_driver.su ./Driver/Src/i2c_driver.d ./Driver/Src/i2c_driver.o ./Driver/Src/i2c_driver.su ./Driver/Src/spi_driver.d ./Driver/Src/spi_driver.o ./Driver/Src/spi_driver.su ./Driver/Src/stm32f4xx.d ./Driver/Src/stm32f4xx.o ./Driver/Src/stm32f4xx.su ./Driver/Src/usart_driver.d ./Driver/Src/usart_driver.o ./Driver/Src/usart_driver.su

.PHONY: clean-Driver-2f-Src

