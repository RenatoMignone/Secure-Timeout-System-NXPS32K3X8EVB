/*
* Linker script for S32K358 with FreeRTOS
* Configured to use ITCM, DTCM, SRAM and Flash memory regions.
*/

/* Defining memory regions */
MEMORY
{
    /* ITCM: Queues and performance-critical data */
    ITCM0      (RWX) : ORIGIN = 0x00000000, LENGTH = 64k
    ITCM2      (RWX) : ORIGIN = 0x00010000, LENGTH = 64k

    /* PFLASH: Main program */
    PFLASH     (RX)  : ORIGIN = 0x00400000, LENGTH = 8M

    /* DFLASH: Non-volatile data */
    DFLASH     (RW)  : ORIGIN = 0x10000000, LENGTH = 128k

    /* DTCM: Stack, heap, and critical data */
    DTCM0      (RW)  : ORIGIN = 0x20000000, LENGTH = 128k
    DTCM2      (RW)  : ORIGIN = 0x21800000, LENGTH = 128k

    /* SRAM: Generic data */
    SRAM_STDBY (RW)  : ORIGIN = 0x20400000, LENGTH = 64k  /* SRAM Standby */
    SRAM0      (RW)  : ORIGIN = 0x20410000, LENGTH = 192k /* Restante SRAM0 */
    SRAM1      (RW)  : ORIGIN = 0x20440000, LENGTH = 256k
    SRAM2      (RW)  : ORIGIN = 0x20480000, LENGTH = 256k

    FLASH      (rx)  : ORIGIN = 0x00400000, LENGTH = 8M
    SRAM       (rw)  : ORIGIN = 0x20400000, LENGTH = 64k + 192k + 256k + 256k

    /* Flash UTEST */
    UTEST      (RX)  : ORIGIN = 0x1B000000, LENGTH = 8k 
}

/* Minimum size for heap and stack */
_Min_Heap_Size = 0x1000;
_Min_Stack_Size = 0x400;    /* Required amount of stack.  Used by main(), then re-used as the interrupt stack after the kernel starts. */

/* Initial stack */
_estack = ORIGIN(DTCM2) + LENGTH(DTCM2);

/* Linker Sections */
SECTIONS
{
    /* ISR Vector */
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
        *(.text*)        
        *(.rodata*)      
        *(.constdata*)   
        _etext = .;      
        KEEP(*(.init))   
    } > PFLASH

    /* Initialized data (RAM) */
    .data :
    {
        . = ALIGN(8);
        _sdata = .;
        *(.data)         
        *(.data.*)       
        *(vtable)
        _edata = .; 
    } > DTCM0 AT > PFLASH

    /* Symbols for copying from FLASH to RAM */
    _sidata = LOADADDR(.data);  /* Load address (FLASH) of .data */

    /* Uninitialized data (RAM) */
    .bss :
    {
        . = ALIGN(8);
        _sbss = .;
        *(.bss)
        *(.bss.*)
        _ebss = .;
    } > DTCM0

    /* Standby ram */
    .standby_ram :
    {
        . = ALIGN(8);
        *(.standby_ram)
    } > SRAM_STDBY

    /* Test data */
    .utest :
    {
        KEEP(*(.utest))
    } > UTEST

    /* Heap */
    .heap :
    {
        . = ALIGN(8);
        PROVIDE(end = .);
        PROVIDE(_end = .);
        _heap_bottom = .;
        . = . + _Min_Heap_Size;
        _heap_top = .;
    } > DTCM0

    /* Stack */
    .stack :
    {
        . = ALIGN(8);
        __stack_start__ = .;
        . = . + _Min_Stack_Size;
        __stack_end__ = .;
    } > DTCM2

    /* Privileged code/data (used by FreeRTOS MPU port) */
    .privileged_functions : {
      __privileged_functions_start__ = .;
      *(.privileged_functions)
      __privileged_functions_end__ = .;
    } > FLASH

    .privileged_data : {
      __privileged_data_start__ = .;
      *(.privileged_data)
      __privileged_data_end__ = .;
    } > SRAM

    /* System call handlers */
    .syscalls : {
      __syscalls_flash_start__ = .;
      *(.syscalls)
      __syscalls_flash_end__ = .;
    } > FLASH

    /* Symbol definitions for FreeRTOS MPU */
    __SRAM_segment_start__ = ORIGIN(SRAM);
    __SRAM_segment_end__   = ORIGIN(SRAM) + LENGTH(SRAM);
    __FLASH_segment_start__ = ORIGIN(FLASH);
    __FLASH_segment_end__   = ORIGIN(FLASH) + LENGTH(FLASH); 
       
    ASSERT(__stack_end__ <= ORIGIN(DTCM2) + LENGTH(DTCM2), "Stack overflow in DTCM2!")

}




/* Entry point */
ENTRY(Reset_Handler)

/* Initial stack */
PROVIDE(_stack = _estack);
