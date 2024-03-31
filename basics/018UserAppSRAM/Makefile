CC=arm-none-eabi-gcc 
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -O0 -ffunction-sections -fdata-sections
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft -T stm32_discovery.ld -Wl,-Map=user_app_sram.map -Wl,--gc-sections

all: user_app_sram.o stm32f4xx.o gpio_driver.o stm32_startup.o usart_driver.o general_purpose_timer.o common_utils.o syscalls.o sysmem.o user_app_sram.elf

user_app_sram.o: user_app_sram.c
	$(CC) $(CFLAGS) -o $@ $^

stm32f4xx.o: stm32f4xx.c
	$(CC) $(CFLAGS) -o $@ $^

gpio_driver.o: gpio_driver.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o: stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

usart_driver.o: usart_driver.c
	$(CC) $(CFLAGS) -o $@ $^	

general_purpose_timer.o: general_purpose_timer.c
	$(CC) $(CFLAGS) -o $@ $^

common_utils.o: common_utils.c
	$(CC) $(CFLAGS) -o $@ $^

syscalls.o: syscalls.c
	$(CC) $(CFLAGS) -o $@ $^

sysmem.o: sysmem.c
	$(CC) $(CFLAGS) -o $@ $^			

user_app_sram.elf: user_app_sram.o stm32f4xx.o gpio_driver.o stm32_startup.o usart_driver.o general_purpose_timer.o common_utils.o syscalls.o sysmem.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf *.o *.elf *.map *.bin
