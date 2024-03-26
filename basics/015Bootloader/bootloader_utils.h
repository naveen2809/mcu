#ifndef INC_BOOTLOADER_UTILS_H_
#define INC_BOOTLOADER_UTILS_H_

#include <stdint.h>

#define CMD_BUF_LEN     100
#define MAX_DATA_LEN    1024
#define RX_BUF_LEN      (MAX_DATA_LEN)
#define DATA_SECTOR     7
#define DATA_ADDRESS    0x08060000UL
#define IMAGE_ADDRESS   0x08010000UL

void start_app(uint32_t sp, uint32_t pc);

void handle_command(void);
void handle_command_get_version(void);
void handle_command_sector_erase(void);
void handle_command_mass_erase(void);
void handle_command_start_app(void);
void handle_command_data_read(void);
void handle_command_data_write(void);
void handle_command_flash_image(void);
void handle_command_soft_reset(void);
void handle_command_system_reset(void);

uint32_t getcommandname(char *src,char *dst,uint32_t len);
uint32_t getarg(char *src,uint32_t len,uint32_t arg_no);
uint32_t getdataindex(char *src,uint32_t len);
uint8_t mystrcmp(char *src,char *dst,uint32_t len);
uint32_t get_power(uint32_t exponent);



#endif /* INC_BOOTLOADER_UTILS_H_ */