/*
 * Linker script for S32K3X8EVB with FreeRTOS
 * This linker script is configured to use the flash and RAM regions defined in the board code.
 */

/* Define memory regions */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00400000, LENGTH = 2M  /* Program flash memory */
    RAM  (rwx) : ORIGIN = 0x20400000, LENGTH = 256k /* RAM */
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
        *(.constdata*)
        _etext = .;
        _sidata = .;
        KEEP(*(.init))  /* Startup initialization */
    } > FLASH

    /* Read-write data section */
    .data :
    {
        . = ALIGN(8);
        _data = .;
        _sdata = .;
        *(vtable)
        *(.data)
        _edata = .;
    } > RAM

    /* Zero-initialized data */
    .bss :
    {
        . = ALIGN(8);
        _bss = .;
        _sbss = .;
        *(.bss)
        _ebss = .;
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
   } >RAM


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


