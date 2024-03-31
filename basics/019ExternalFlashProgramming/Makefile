CC=arm-none-eabi-gcc 
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -O0 -ffunction-sections -fdata-sections
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft -T stm32_discovery.ld -Wl,-Map=bootloader.map -Wl,--gc-sections

all:  ext_flash_programming.o stm32f4xx.o gpio_driver.o stm32_startup.o usart_driver.o common_utils.o general_purpose_timer.o syscalls.o sysmem.o at45dbxx_flash_memory_driver.o spi_driver.o ext_flash_programming.elf

ext_flash_programming.o: ext_flash_programming.c
	$(CC) $(CFLAGS) -o $@ $^

stm32f4xx.o: stm32f4xx.c
	$(CC) $(CFLAGS) -o $@ $^

gpio_driver.o: gpio_driver.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o: stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

usart_driver.o: usart_driver.c
	$(CC) $(CFLAGS) -o $@ $^

common_utils.o: common_utils.c
	$(CC) $(CFLAGS) -o $@ $^

general_purpose_timer.o: general_purpose_timer.c
	$(CC) $(CFLAGS) -o $@ $^

at45dbxx_flash_memory_driver.o: at45dbxx_flash_memory_driver.c
	$(CC) $(CFLAGS) -o $@ $^

spi_driver.o: spi_driver.c
	$(CC) $(CFLAGS) -o $@ $^

syscalls.o: syscalls.c
	$(CC) $(CFLAGS) -o $@ $^

sysmem.o: sysmem.c
	$(CC) $(CFLAGS) -o $@ $^	

ext_flash_programming.elf: ext_flash_programming.o stm32f4xx.o gpio_driver.o stm32_startup.o usart_driver.o common_utils.o general_purpose_timer.o syscalls.o sysmem.o at45dbxx_flash_memory_driver.o spi_driver.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf *.o *.elf *.map
