#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bootloader_utils.h"
#include "flash_driver.h"

//Linker script symbols
extern uint32_t __bootrom_start__;
extern uint32_t __bootrom_len__;
extern uint32_t __approm_start__;
extern uint32_t __approm_len__;

char CmdBuffer[CMD_BUF_LEN];
uint32_t CmdLen;
uint8_t *pData;
char RxBuffer[RX_BUF_LEN];

void handle_command(void)
{
	char command_name[CMD_BUF_LEN];
	uint32_t command_name_length;

	CmdBuffer[CmdLen-1] = '\0';

	command_name_length = getcommandname(CmdBuffer,command_name,CmdLen);

	if(mystrcmp(command_name,"version",command_name_length))
	{
		handle_command_get_version();
	}
	else if(mystrcmp(command_name,"sector_erase",command_name_length))
	{
		handle_command_sector_erase();
	}
	else if(mystrcmp(command_name,"mass_erase",command_name_length))
	{
		handle_command_mass_erase();
	}
	else if(mystrcmp(command_name,"start_app",command_name_length))
	{
		handle_command_start_app();
	}
	else if(mystrcmp(command_name,"start_app_sram",command_name_length))
	{
		handle_command_start_app_sram();
	}
    else if(mystrcmp(command_name,"data_read",command_name_length))
	{
		handle_command_data_read();
	}
    else if(mystrcmp(command_name,"data_write",command_name_length))
	{
		handle_command_data_write();
	}
    else if(mystrcmp(command_name,"flash_image",command_name_length))
	{
		handle_command_flash_image();
	}
	else if(mystrcmp(command_name,"flash_image_sram",command_name_length))
	{
		handle_command_flash_image_sram();
	}
    else if(mystrcmp(command_name,"soft_reset",command_name_length))
    {
        handle_command_soft_reset();
    }
    else if(mystrcmp(command_name,"system_reset",command_name_length))
    {
        handle_command_system_reset();
    }

	return;
}

void handle_command_get_version(void)
{
	printf("Bootloader v0.1\r\n");

	return;
}

void handle_command_sector_erase(void)
{
	uint32_t sector;

	sector = getarg(CmdBuffer,CmdLen,1);
	if(sector >= 0 && sector<=7)
	{
        flash_sector_erase(sector);
        printf("Sector %ld erased successfully\r\n",sector);
	}
	else
	{
		printf("Invalid sector\r\n");
	}

	return;
}

void handle_command_mass_erase(void)
{
	printf("Erasing the entire flash...\r\n");
    flash_mass_erase();
	return;
}

void handle_command_start_app(void)
{
    uint32_t *app_ptr;
	uint32_t app_sp;
	uint32_t app_pc;
    
    printf("Starting application...\r\n");

    //Jumping to the application program by calling its reset handler
	app_ptr = (uint32_t *) &__approm_start__;
	app_sp = (volatile uint32_t) app_ptr[0];
	app_pc = (volatile uint32_t) app_ptr[1];
	
	start_app(app_sp,app_pc);

	return;
}

void start_app(uint32_t sp, uint32_t pc)
{
	__asm volatile ("MSR MSP, R0");
	__asm volatile ("BX R1"); 	
}

void handle_command_start_app_sram(void)
{
    uint32_t *app_ptr;
	uint32_t app_sp;
	uint32_t app_pc;

	uint32_t i, image_length;
	uint8_t *pSRAM;
	uint8_t *pFLASH;
	uint8_t data_byte;

	//Copy Image from Flash to SRAM
	image_length = getarg(CmdBuffer,CmdLen,1);
	pSRAM = (uint8_t *) APP_SRAM_START;
	pFLASH = (uint8_t *) IMAGE_ADDRESS_SRAM;
	for(i=0;i<image_length;i++)
	{
		flash_read(&data_byte,(uint8_t*)pFLASH,1);
		*pSRAM = data_byte;
		pFLASH++;
		pSRAM++;
	}
    
    printf("Starting application...\r\n");

    //Jumping to the application program by calling its reset handler
	app_ptr = (uint32_t *) APP_SRAM_START;
	app_sp = (volatile uint32_t) app_ptr[0];
	app_pc = (volatile uint32_t) app_ptr[1];
	
	start_app(app_sp,app_pc);

	return;
}

void handle_command_soft_reset(void)
{
    uint32_t *app_ptr;
	uint32_t app_sp;
	uint32_t app_pc;
    
    printf("Resetting...\r\n");

    //Jumping to the application program by calling its reset handler
	app_ptr = (uint32_t *) &__bootrom_start__;
	app_sp = (volatile uint32_t) app_ptr[0];
	app_pc = (volatile uint32_t) app_ptr[1];
	
	start_app(app_sp,app_pc);

	return;
}

void handle_command_system_reset(void)
{
    uint32_t *pAIRCR = (uint32_t *) 0xE000ED0CUL;
    
    printf("Initiating System Reset...\r\n");

    *pAIRCR = (0x5FA << 16) | (1 << 2);

    while(1)
    {

    }

	return;
}

void handle_command_data_read(void)
{
    uint32_t RxLen;
    
    RxLen = getarg(CmdBuffer,CmdLen,1);

    if(RxLen>=0 && RxLen <=MAX_DATA_LEN)
    {
        flash_read((uint8_t *)RxBuffer,(uint8_t*)DATA_ADDRESS,RxLen);
        printf("%ld read bytes from flash\r\n",RxLen);
    }
    else
    {
        printf("Invalid Rx Length (Max Length: %d bytes)\r\n",MAX_DATA_LEN);
    }

    return;
}

void handle_command_data_write(void)
{
    uint32_t TxData, TxLen,i;

    pData = (uint8_t *) DATA_ADDRESS;

    TxData = getarg(CmdBuffer,CmdLen,1);
    TxLen = getarg(CmdBuffer,CmdLen,2);

    if((TxData>=0 && TxData<=255) && (TxLen>=0 && TxLen<=MAX_DATA_LEN))
    {
        for(i=0;i<TxLen;i++)
        {
            flash_write((uint8_t *)&TxData,(pData+i),1);
        }
        printf("Data written to flash\r\n");
    }
    else
    {
        if(!(TxData>=0 && TxData<=255))
        {
            printf("Invalid Tx Data\r\n");
        }

        if(!(TxLen>=0 && TxLen<=MAX_DATA_LEN))
        {
            printf("Invalid Tx Length\r\n");
        }
    }

    return;
}

void handle_command_flash_image(void)
{
    uint32_t SequenceNum, TxLen,i;
    uint8_t DataIndex;

    SequenceNum = getarg(CmdBuffer,CmdLen,1);
    TxLen = getarg(CmdBuffer,CmdLen,2);
    DataIndex = getdataindex(CmdBuffer,CmdLen);

    if((SequenceNum>=0) && (TxLen>=0 && TxLen<=MAX_DATA_LEN))
    {
        if(SequenceNum == 0)
        {
            pData = (uint8_t *) IMAGE_ADDRESS;
        }
        
        for(i=0;i<TxLen;i++)
        {
            flash_write((uint8_t *)&CmdBuffer[DataIndex],pData,1);
            DataIndex++;
            pData++;
        }
        printf("Cmd: flash_image Data written to flash\r\n");
    }
    else
    {
        if(!(SequenceNum>=0))
        {
            printf("Invalid Sequence Number\r\n");
        }

        if(!(TxLen>=0 && TxLen<=MAX_DATA_LEN))
        {
            printf("Invalid Tx Length\r\n");
        }
    }

    return;
}

void handle_command_flash_image_sram(void)
{
    uint32_t SequenceNum, TxLen,i;
    uint8_t DataIndex;

    SequenceNum = getarg(CmdBuffer,CmdLen,1);
    TxLen = getarg(CmdBuffer,CmdLen,2);
    DataIndex = getdataindex(CmdBuffer,CmdLen);

    if((SequenceNum>=0) && (TxLen>=0 && TxLen<=MAX_DATA_LEN))
    {
        if(SequenceNum == 0)
        {
            pData = (uint8_t *) IMAGE_ADDRESS_SRAM;
        }
        
        for(i=0;i<TxLen;i++)
        {
            flash_write((uint8_t *)&CmdBuffer[DataIndex],pData,1);
            DataIndex++;
            pData++;
        }
        printf("Cmd: flash_image Data written to flash\r\n");
    }
    else
    {
        if(!(SequenceNum>=0))
        {
            printf("Invalid Sequence Number\r\n");
        }

        if(!(TxLen>=0 && TxLen<=MAX_DATA_LEN))
        {
            printf("Invalid Tx Length\r\n");
        }
    }

    return;
}

uint32_t getcommandname(char *src,char *dst,uint32_t len)
{
	uint32_t i, cmd_length;
	cmd_length = 0;

	for(i=0;i<len;i++)
	{
		if(src[i] == ' ')
		{
			dst[i] = '\0';
			cmd_length++;
			break;
		}
		else
		{
			dst[i] = src[i];
			cmd_length++;
		}
	}

	return cmd_length;
}

uint32_t getarg(char *src,uint32_t len,uint32_t arg_no)
{
	uint32_t i, j, arg_length, arg, power;
	char arg_string[100];

    memset(arg_string,0,100);

	i = 0;
	arg_length = 0;
	arg = 0;

    for(j=0;j<arg_no;j++)
    {
        while(src[i] != ' ')
        {
            i++;
        }

        i++;
    }

	while(i<len)
	{
		if(src[i] == ' ' || src[i] == '\0')
		{
			arg_string[arg_length] = '\0';
			arg_length++;
			i++;
			break;
		}
		else
		{
			arg_string[arg_length] = src[i];
			arg_length++;
			i++;
		}
	}

	for(i=arg_length-1;i>0;i--)
	{
		power = get_power(i-1);
		arg += (arg_string[arg_length-1-i]-48)*power;
	}
    

	return arg;
}

uint32_t getdataindex(char *src,uint32_t len)
{
    uint32_t i, j;

	i = 0;

    for(j=0;j<3;j++)
    {
        while(src[i] != ' ')
        {
            i++;
        }

        i++;
    }

    return i;
}

uint8_t mystrcmp(char *src,char *dst,uint32_t len)
{
	uint32_t i,count;

	count = 0;
	for(i=0;i<len;i++)
	{
		if(src[i] == dst[i])
		{
			count++;
		}
		else
		{
			break;
		}
	}

	if(count == len)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint32_t get_power(uint32_t exponent)
{
	uint32_t i, num = 1;

	for(i=0;i<exponent;i++)
	{
		num = num * 10;
	}

	return num;
}