ENTRY(Reset_Handler)

MEMORY
{
    BOOTROM (rx): ORIGIN = 0x08000000, LENGTH = 32K
    APPROM (rx): ORIGIN = 0x08008000, LENGTH = 480K
    SRAM  (rwx): ORIGIN = 0x20000000, LENGTH = 256K
}

__bootrom_start__ = ORIGIN(BOOTROM);
__bootrom_len__ = LENGTH(BOOTROM);
__approm_start__ = ORIGIN(APPROM);
__approm_len__ = LENGTH(APPROM);

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        _stext = .;
        KEEP(*(.isr_vector))
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } > BOOTROM

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > SRAM AT> BOOTROM

    _sidata = LOADADDR(.data);

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > SRAM
}