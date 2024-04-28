CC=arm-none-eabi-gcc 
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -std=gnu11 -Wall -O0 -ffunction-sections -fdata-sections
LDFLAGS= -mcpu=$(MACH) -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -T Src/stm32_discovery.ld -Wl,-Map=$(BUILDDIR)/$(EXECUTABLE).map -Wl,--gc-sections

SRCS = $(shell find . -name '*.c')
OBJS := $(patsubst %.c,%.o,$(SRCS))
EXECUTABLE = freertos_build
INC_PATH = -I Inc/  -I ThirdParty/FreeRTOS/include -I ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F
BUILDDIR = build

vpath %.c Src ThirdParty/FreeRTOS/ ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/ ThirdParty/FreeRTOS/portable/MemMang/
vpath %.o Src ThirdParty/FreeRTOS/ ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/ ThirdParty/FreeRTOS/portable/MemMang/
vpath %.h Inc ThirdParty/FreeRTOS/include ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F/

all: dir $(BUILDDIR)/$(EXECUTABLE).elf

dir:
	mkdir -p $(BUILDDIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INC_PATH) -c $<  -o $@

$(BUILDDIR)/$(EXECUTABLE).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf $(BUILDDIR)
