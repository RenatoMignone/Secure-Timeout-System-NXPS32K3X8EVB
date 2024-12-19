/*
 * Linker script for S32K3X8EVB with FreeRTOS
 * This linker script is configured to use the flash and RAM regions defined in the board code.
 */

/* Define memory regions */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00400000, LENGTH = 2M  /* Program flash memory */
    RAM  (rwx) : ORIGIN = 0x20400000, LENGTH = 256K /* RAM */
}
ENTRY(Reset_Handler)

_Min_Heap_Size = 0x8;        /* Not used as building heap_4.c */
_Min_Stack_Size = 0x400 ;       /* Required amount of stack.  Used by main(), then re-used as the interrupt stack after the kernel starts. */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Define sections */
SECTIONS
{
    /* Start of code and read-only data */
    .isr_vector :
    {
        __vector_table = .;
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    .text :
    {
        *(.text*)       /* Code */
        *(.rodata*)     /* Read-only data */
        KEEP(*(.init))  /* Startup initialization */
    } > FLASH

    /* Read-write data section */
    .data :
    {
        . = ALIGN(8);
        _sdata = .;               /* Start address of data section */
        *(.data*)                 /* Data */
        _edata = .;               /* End address of data section */
    } > RAM AT > FLASH

    /* Zero-initialized data */
    .bss :
    {
        . = ALIGN(8);
        _sbss = .;                /* Start address of BSS */
        *(.bss*)                  /* BSS */
        *(COMMON)                 /* Common variables */
        _ebss = .;                /* End address of BSS */
    } > RAM

    /* Stack section */
    ._user_stack :
    {
        . = ALIGN(8);
        _estack = .;              /* End of stack (initial stack pointer) */
        . += 0x4000;              /* Stack size (adjust as needed) */
    } > RAM

    /* Heap section */
    .heap :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        _heap_bottom = .;
        . = . + _Min_Heap_Size;
        _heap_top = .;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
     } > RAM


   __StackTop = ORIGIN(RAM) + LENGTH(RAM);
   __StackLimit = __StackTop - _Min_Stack_Size;

    ASSERT(__StackLimit >= _heap_top, "region RAM overflowed with stack")



    /* End of memory layout */
    _end = .;


}

/* Define entry point */
ENTRY(Reset_Handler)

/* Provide stack information */
PROVIDE(_stack = _estack);


