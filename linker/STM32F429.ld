/* Label for the program's entry point */
ENTRY(Reset_Handler)

/* Define the end of RAM and limit of stack memory */
/* (245KB SRAM on the STM32F429xx line, 256kb = 0x40000) */
/* (RAM starts at address 0x20000000) */

_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Set minimum size for stack and dynamic memory */
/* (The linker will generate an error if there is
 * less than this much RAM leftover.) */
 /* 1KB */
_Min_Leftover_RAM = 0x400;

MEMORY
{
    CCMRAM    (xrw)    : ORIGIN = 0x10000000,   LENGTH = 64K /* Core Coupled Memory */
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 2048K
    RAM (rxw)       : ORIGIN = 0x20000000, LENGTH = 192K
}

SECTIONS
{
    /* Some symbol;
    * '.' means location counter
    * '*' mean that wildcard matches any file name
    *       ex: *(.text) mean all .text input sections in all input files.
    * '>region'
    * AT(arg): this specifies the exact load address of the section
    */

    /* The vector table goes at the start of flash */
    .vector_table :
    {
        . = ALIGN(4);
        KEEP (*(.vector_table))
        . = ALIGN(4);
    } >FLASH

    /* The 'text' section contains the main program code */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
    } >FLASH

    /* Sections required by the standard libraries */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH

    .ARM :
    {
        *(.ARM.exidx*)
    } >FLASH

    /* The 'rosdata' section contains read-only data,
    * constants, strings, information that won't change */
    .rosdata :
    {
        . = ALIGN(4);
        *(.rosdata)
        *(.rosdata*)
        . = ALIGN(4);
    } >FLASH

    /* The 'data' section is space set aside in RAM for
    * things like variable, with can change. */
    _sidata = .;
    .data : AT(_sidata)
    {
        . = ALIGN(4);
        /* Mark start/end locations for the 'data' section. */
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .;
        . = ALIGN(4);
    } >RAM

    /* The 'bss' section is similar to the 'data' sections,
    * but its space is initialized to all 0s at the
    * start of the program */
    .bss :
    {
        . = ALIGN(4);
        /* Also mark the start/end of the bss section. */
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM

    /*Mark the end of statically-allocated RAM. */
    end = .;
    _end = end;
    __end = end;

    /* Space set aside for the application's  heap/stack */
    .dynamic_allocations :
    {
        . = ALIGN(4);
        _ssystem_ram = .;
        . = . + _Min_Leftover_RAM;
        . = ALIGN(4);
        _esystem_ram = .;
    } >RAM
}