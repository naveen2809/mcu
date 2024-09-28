################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/dma_driver.c \
../Drivers/Src/gpio_driver.c \
../Drivers/Src/stm32f4xx.c \
../Drivers/Src/usart_driver.c 

OBJS += \
./Drivers/Src/dma_driver.o \
./Drivers/Src/gpio_driver.o \
./Drivers/Src/stm32f4xx.o \
./Drivers/Src/usart_driver.o 

C_DEPS += \
./Drivers/Src/dma_driver.d \
./Drivers/Src/gpio_driver.d \
./Drivers/Src/stm32f4xx.d \
./Drivers/Src/usart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/%.o Drivers/Src/%.su: ../Drivers/Src/%.c Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/Config" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/Sample/FreeRTOS" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/SEGGER/SystemView/SEGGER" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/Drivers/Inc" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/FreeRTOS/include" -I"/home/naveen/Code/mcu/basics/015FreeRTOSProjects/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-Drivers-2f-Src

clean-Drivers-2f-Src:
	-$(RM) ./Drivers/Src/dma_driver.d ./Drivers/Src/dma_driver.o ./Drivers/Src/dma_driver.su ./Drivers/Src/gpio_driver.d ./Drivers/Src/gpio_driver.o ./Drivers/Src/gpio_driver.su ./Drivers/Src/stm32f4xx.d ./Drivers/Src/stm32f4xx.o ./Drivers/Src/stm32f4xx.su ./Drivers/Src/usart_driver.d ./Drivers/Src/usart_driver.o ./Drivers/Src/usart_driver.su

.PHONY: clean-Drivers-2f-Src

