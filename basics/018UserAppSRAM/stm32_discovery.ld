ENTRY(Reset_Handler)

MEMORY
{
    BOOTROM (rx): ORIGIN = 0x08000000, LENGTH = 64K
    APPROM (rx): ORIGIN = 0x08010000, LENGTH = 448K
    SRAM1  (rwx): ORIGIN = 0x20000000, LENGTH = 64K
    SRAM2  (rwx): ORIGIN = 0x20010000, LENGTH = 128K
    SRAM3  (rwx): ORIGIN = 0x20030000, LENGTH = 64K
}
__app_sram_start__ = ORIGIN(SRAM2);
__app_sram_len__ = LENGTH(SRAM2);

/* Highest address of the user mode stack */
_estack = ORIGIN(SRAM2) + LENGTH(SRAM2); /* end of "RAM" Ram type memory */

_Min_Heap_Size = 0x200; /* required amount of heap */
_Min_Stack_Size = 0x400; /* required amount of stack */

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        _stext = .;
        KEEP(*(.isr_vector))
        *(.text)
        *(.text*)
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        *(.eh_frame)
        KEEP (*(.init))
        KEEP (*(.fini))
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } >SRAM2

    .ARM.extab   : {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
    } >SRAM2

    .ARM : {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(4);
    } >SRAM2

    .preinit_array     :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
    } >SRAM2

    .init_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
    } >SRAM2

    .fini_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(4);
    } >SRAM2
    
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >SRAM2

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        _end = _ebss;
    } >SRAM2
}