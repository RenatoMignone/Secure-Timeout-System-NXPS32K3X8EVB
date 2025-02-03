/*
 * Linker script for S32K358 with FreeRTOS
 * Configured to use ITCM, DTCM, SRAM, and Flash memory regions.
 */

/* Defining memory regions */
MEMORY
{
    /* ITCM: Queues and performance-critical data */
    ITCM0      (RWX) : ORIGIN = 0x00000000, LENGTH = 64K
    ITCM2      (RWX) : ORIGIN = 0x00010000, LENGTH = 64K

    /* PFLASH: Main program */
    PFLASH     (RX)  : ORIGIN = 0x00400000, LENGTH = 8M

    /* DFLASH: Non-volatile data */
    DFLASH     (RW)  : ORIGIN = 0x10000000, LENGTH = 128K

    /* DTCM: Stack, heap, and critical data */
    DTCM0      (RW)  : ORIGIN = 0x20000000, LENGTH = 128K
    DTCM2      (RW)  : ORIGIN = 0x21800000, LENGTH = 128K

    /* SRAM: Generic data */
    SRAM_STDBY (RW)  : ORIGIN = 0x20400000, LENGTH = 64K  /* SRAM Standby */
    SRAM0      (RW)  : ORIGIN = 0x20410000, LENGTH = 192K /* Remaining SRAM0 */
    SRAM1      (RW)  : ORIGIN = 0x20440000, LENGTH = 256K
    SRAM2      (RW)  : ORIGIN = 0x20480000, LENGTH = 256K

    /* Combined SRAM region */
    SRAM       (RW)  : ORIGIN = 0x20400000, LENGTH = 704K /* Total SRAM */

    /* Flash UTEST */
    UTEST      (RX)  : ORIGIN = 0x1B000000, LENGTH = 8K
}

/* Minimum size for heap and stack */
_Min_Heap_Size = 0x2000;   /* Increased to 8 KB for more dynamic memory */
_Min_Stack_Size = 0x800;   /* Increased to 2 KB for better task handling */

/* Initial stack pointer */
_estack = ORIGIN(DTCM2) + LENGTH(DTCM2);

/* Linker Sections */
SECTIONS
{
    /* ISR Vector Table */
    .isr_vector :
    {
        __vector_table = .;
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > ITCM0

    /* Readonly code and data section */
    .text :
    {
        __coderom_start__ = .;
        *(.text*)         /* Code */
        *(.rodata*)       /* Read-only data */
        *(.constdata*)    /* Constant data */
        _etext = .;       /* End of text section */
        KEEP(*(.init))    /* Initialization code */
    } > PFLASH

    /* Initialized data (RAM) */
    .data :
    {
        . = ALIGN(8);
        _sdata = .;       /* Start of initialized data in RAM */
        *(.data)          /* Initialized global/static variables */
        *(.data.*)        /* Section-specific initialized data */
        *(vtable)         /* Vector table */
        _edata = .;       /* End of initialized data in RAM */
    } > DTCM0 AT > PFLASH

    /* Symbols for copying from FLASH to RAM */
    _sidata = LOADADDR(.data); /* Load address (FLASH) of .data */

    /* Uninitialized data (RAM) */
    .bss :
    {
        . = ALIGN(8);
        _sbss = .;        /* Start of uninitialized data */
        *(.bss)           /* Uninitialized global/static variables */
        *(.bss.*)         /* Section-specific uninitialized data */
        _ebss = .;        /* End of uninitialized data */
    } > DTCM0

    /* Standby RAM */
    .standby_ram :
    {
        . = ALIGN(8);
        *(.standby_ram)   /* Data specific to standby mode */
    } > SRAM_STDBY

    /* Test data */
    .utest :
    {
        KEEP(*(.utest))   /* Test-related data */
    } > UTEST

    /* Heap */
    .heap :
    {
        . = ALIGN(8);
        PROVIDE(end = .); /* End of all sections */
        PROVIDE(_end = .);
        _heap_bottom = .; /* Start of heap */
        . = . + _Min_Heap_Size; /* Allocate minimum heap size */
        _heap_top = .;    /* End of heap */
    } > DTCM0

    /* Stack */
    .stack :
    {
        . = ALIGN(8);
        __stack_start__ = .; /* Start of stack */
        . = . + _Min_Stack_Size; /* Allocate minimum stack size */
        __stack_end__ = .;   /* End of stack */
    } > DTCM2

    /* Privileged code/data (used by FreeRTOS MPU port) */
    .privileged_functions :
    {
        __privileged_functions_start__ = .;
        *(.privileged_functions) /* Privileged functions */
        __privileged_functions_end__ = .;
    } > PFLASH

    .privileged_data :
    {
        __privileged_data_start__ = .;
        *(.privileged_data) /* Privileged data */
        __privileged_data_end__ = .;
    } > SRAM

    /* System call handlers */
    .syscalls :
    {
        __syscalls_flash_start__ = .;
        *(.syscalls) /* System call handlers */
        __syscalls_flash_end__ = .;
    } > PFLASH

    /* Symbol definitions for FreeRTOS MPU */
    __SRAM_segment_start__ = ORIGIN(SRAM);
    __SRAM_segment_end__   = ORIGIN(SRAM) + LENGTH(SRAM);
    __FLASH_segment_start__ = ORIGIN(PFLASH);
    __FLASH_segment_end__   = ORIGIN(PFLASH) + LENGTH(PFLASH);

    /* Assertions for safety */
    ASSERT(__stack_end__ <= ORIGIN(DTCM2) + LENGTH(DTCM2), "Stack overflow in DTCM2!")
    ASSERT(_heap_top <= ORIGIN(DTCM0) + LENGTH(DTCM0), "Heap overflow in DTCM0!")
}

/* Entry point */
ENTRY(Reset_Handler)

/* Initial stack pointer */
PROVIDE(_stack = _estack);