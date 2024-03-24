CC=arm-none-eabi-gcc 
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -O0 -ffunction-sections -fdata-sections
#LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft -T stm32_discovery.ld -Wl,-Map=gpio_test.map -nostdlib -Wl,--gc-sections
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft -T stm32_discovery.ld -Wl,-Map=gpio_test.map -Wl,--gc-sections

all: gpio_test.o stm32f4xx.o gpio_driver.o stm32_startup.o gpio_test.elf

gpio_test.o: gpio_test.c
	$(CC) $(CFLAGS) -o $@ $^

stm32f4xx.o: stm32f4xx.c
	$(CC) $(CFLAGS) -o $@ $^

gpio_driver.o: gpio_driver.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o: stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

gpio_test.elf: gpio_test.o stm32f4xx.o gpio_driver.o stm32_startup.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf *.o *.elf *.map
