CC=arm-none-eabi-gcc
ASSEM=arm-none-eabi-as
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -std=gnu11 -Wall -O0 -ffunction-sections -fdata-sections
LDFLAGS= -mcpu=$(MACH) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -T src/stm32_discovery.ld -Wl,-Map=$(BUILD_DIR)/$(EXECUTABLE).map -Wl,--gc-sections 
BUILD_DIR= build
EXECUTABLE= main
SOURCES= $(shell find . -name '*.c')
OBJECTS= $(patsubst %.c,%.o,$(SOURCES))
OBJECTS += ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o
INC_DIRS= -Iinc -IThirdParty/FreeRTOS/include -IThirdParty/FreeRTOS/portable/GCC/ARM_CM4F -IThirdParty/SEGGER/Config -IThirdParty/SEGGER/SEGGER -IThirdParty/SEGGER/SEGGER/Syscalls -IThirdParty/SEGGER/OS

vpath %.c src ThirdParty/FreeRTOS ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F ThirdParty/FreeRTOS/portable/MemMang ThirdParty/SEGGER/SEGGER ThirdParty/SEGGER/SEGGER/Syscalls ThirdParty/SEGGER/OS ThirdParty/SEGGER/Config
vpath %.o src ThirdParty/FreeRTOS ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F ThirdParty/FreeRTOS/portable/MemMang ThirdParty/SEGGER/SEGGER ThirdParty/SEGGER/SEGGER/Syscalls ThirdParty/SEGGER/OS ThirdParty/SEGGER/Config

all: dir $(BUILD_DIR)/$(EXECUTABLE).elf

dir:
	mkdir -p $(BUILD_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INC_DIRS) $^ -o $@ 

ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o: ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S
	$(CC) -mcpu=$(MACH) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -x assembler-with-cpp -IThirdParty/SEGGER/Config -IThirdParty/SEGGER/SEGGER -Wall -O0 -ffunction-sections -fdata-sections -c -o $@ $^

$(BUILD_DIR)/$(EXECUTABLE).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm src/*.o 
	rm ThirdParty/FreeRTOS/*.o 
	rm ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/*.o 
	rm ThirdParty/FreeRTOS/portable/MemMang/*.o
	rm ThirdParty/SEGGER/SEGGER/*.o
	rm ThirdParty/SEGGER/SEGGER/Syscalls/*.o
	rm ThirdParty/SEGGER/OS/*.o 
	rm ThirdParty/SEGGER/Config/*.o 


.PHONY: dir clean all