CC=arm-none-eabi-gcc 
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -O0 -ffunction-sections -fdata-sections
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft -T stm32_discovery.ld -Wl,-Map=flash_programming.map -nostdlib -Wl,--gc-sections

all: flash_programming.o flash_driver.o stm32f4xx.o stm32_startup.o gpio_driver.o flash_programming.elf

flash_programming.o: flash_programming.c
	$(CC) $(CFLAGS) -o $@ $^

flash_driver.o: flash_driver.c
	$(CC) $(CFLAGS) -o $@ $^

stm32f4xx.o: stm32f4xx.c
	$(CC) $(CFLAGS) -o $@ $^

gpio_driver.o: gpio_driver.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o: stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

flash_programming.elf: flash_programming.o  flash_driver.o stm32f4xx.o gpio_driver.o stm32_startup.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf *.o *.elf *.map
