################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/Src/at24cxx_eeprom_driver.c \
../Driver/Src/at45dbxx_flash_memory_driver.c \
../Driver/Src/common_utils.c \
../Driver/Src/dht11_driver.c \
../Driver/Src/dma_driver.c \
../Driver/Src/ds3231_rtc_driver.c \
../Driver/Src/external_interrupt_config.c \
../Driver/Src/flash_driver.c \
../Driver/Src/general_purpose_timer.c \
../Driver/Src/gpio_driver.c \
../Driver/Src/i2c_driver.c \
../Driver/Src/lcd_driver_pcf8574.c \
../Driver/Src/lm75a_temp_sensor_driver.c \
../Driver/Src/nRFL01_driver.c \
../Driver/Src/os_kernel.c \
../Driver/Src/os_utilities.c \
../Driver/Src/rtc_driver.c \
../Driver/Src/spi_driver.c \
../Driver/Src/stm32f4xx.c \
../Driver/Src/timer_driver.c \
../Driver/Src/usart_driver.c \
../Driver/Src/w5500_driver.c 

OBJS += \
./Driver/Src/at24cxx_eeprom_driver.o \
./Driver/Src/at45dbxx_flash_memory_driver.o \
./Driver/Src/common_utils.o \
./Driver/Src/dht11_driver.o \
./Driver/Src/dma_driver.o \
./Driver/Src/ds3231_rtc_driver.o \
./Driver/Src/external_interrupt_config.o \
./Driver/Src/flash_driver.o \
./Driver/Src/general_purpose_timer.o \
./Driver/Src/gpio_driver.o \
./Driver/Src/i2c_driver.o \
./Driver/Src/lcd_driver_pcf8574.o \
./Driver/Src/lm75a_temp_sensor_driver.o \
./Driver/Src/nRFL01_driver.o \
./Driver/Src/os_kernel.o \
./Driver/Src/os_utilities.o \
./Driver/Src/rtc_driver.o \
./Driver/Src/spi_driver.o \
./Driver/Src/stm32f4xx.o \
./Driver/Src/timer_driver.o \
./Driver/Src/usart_driver.o \
./Driver/Src/w5500_driver.o 

C_DEPS += \
./Driver/Src/at24cxx_eeprom_driver.d \
./Driver/Src/at45dbxx_flash_memory_driver.d \
./Driver/Src/common_utils.d \
./Driver/Src/dht11_driver.d \
./Driver/Src/dma_driver.d \
./Driver/Src/ds3231_rtc_driver.d \
./Driver/Src/external_interrupt_config.d \
./Driver/Src/flash_driver.d \
./Driver/Src/general_purpose_timer.d \
./Driver/Src/gpio_driver.d \
./Driver/Src/i2c_driver.d \
./Driver/Src/lcd_driver_pcf8574.d \
./Driver/Src/lm75a_temp_sensor_driver.d \
./Driver/Src/nRFL01_driver.d \
./Driver/Src/os_kernel.d \
./Driver/Src/os_utilities.d \
./Driver/Src/rtc_driver.d \
./Driver/Src/spi_driver.d \
./Driver/Src/stm32f4xx.d \
./Driver/Src/timer_driver.d \
./Driver/Src/usart_driver.d \
./Driver/Src/w5500_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Driver/Src/%.o Driver/Src/%.su: ../Driver/Src/%.c Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I/home/naveen/Code/mcu/basics/011GPIODriver/Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Driver-2f-Src

clean-Driver-2f-Src:
	-$(RM) ./Driver/Src/at24cxx_eeprom_driver.d ./Driver/Src/at24cxx_eeprom_driver.o ./Driver/Src/at24cxx_eeprom_driver.su ./Driver/Src/at45dbxx_flash_memory_driver.d ./Driver/Src/at45dbxx_flash_memory_driver.o ./Driver/Src/at45dbxx_flash_memory_driver.su ./Driver/Src/common_utils.d ./Driver/Src/common_utils.o ./Driver/Src/common_utils.su ./Driver/Src/dht11_driver.d ./Driver/Src/dht11_driver.o ./Driver/Src/dht11_driver.su ./Driver/Src/dma_driver.d ./Driver/Src/dma_driver.o ./Driver/Src/dma_driver.su ./Driver/Src/ds3231_rtc_driver.d ./Driver/Src/ds3231_rtc_driver.o ./Driver/Src/ds3231_rtc_driver.su ./Driver/Src/external_interrupt_config.d ./Driver/Src/external_interrupt_config.o ./Driver/Src/external_interrupt_config.su ./Driver/Src/flash_driver.d ./Driver/Src/flash_driver.o ./Driver/Src/flash_driver.su ./Driver/Src/general_purpose_timer.d ./Driver/Src/general_purpose_timer.o ./Driver/Src/general_purpose_timer.su ./Driver/Src/gpio_driver.d ./Driver/Src/gpio_driver.o ./Driver/Src/gpio_driver.su ./Driver/Src/i2c_driver.d ./Driver/Src/i2c_driver.o ./Driver/Src/i2c_driver.su ./Driver/Src/lcd_driver_pcf8574.d ./Driver/Src/lcd_driver_pcf8574.o ./Driver/Src/lcd_driver_pcf8574.su ./Driver/Src/lm75a_temp_sensor_driver.d ./Driver/Src/lm75a_temp_sensor_driver.o ./Driver/Src/lm75a_temp_sensor_driver.su ./Driver/Src/nRFL01_driver.d ./Driver/Src/nRFL01_driver.o ./Driver/Src/nRFL01_driver.su ./Driver/Src/os_kernel.d ./Driver/Src/os_kernel.o ./Driver/Src/os_kernel.su ./Driver/Src/os_utilities.d ./Driver/Src/os_utilities.o ./Driver/Src/os_utilities.su ./Driver/Src/rtc_driver.d ./Driver/Src/rtc_driver.o ./Driver/Src/rtc_driver.su ./Driver/Src/spi_driver.d ./Driver/Src/spi_driver.o ./Driver/Src/spi_driver.su ./Driver/Src/stm32f4xx.d ./Driver/Src/stm32f4xx.o ./Driver/Src/stm32f4xx.su ./Driver/Src/timer_driver.d ./Driver/Src/timer_driver.o ./Driver/Src/timer_driver.su ./Driver/Src/usart_driver.d ./Driver/Src/usart_driver.o ./Driver/Src/usart_driver.su ./Driver/Src/w5500_driver.d ./Driver/Src/w5500_driver.o ./Driver/Src/w5500_driver.su

.PHONY: clean-Driver-2f-Src

