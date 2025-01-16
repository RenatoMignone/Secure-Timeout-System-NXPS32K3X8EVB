/*
 * Linker script per la S32K358 con FreeRTOS
 * Configurato per utilizzare le regioni di memoria ITCM, DTCM, SRAM e Flash definite.
 */

/* Definizione delle regioni di memoria */
MEMORY
{
    /* ITCM: Code e dati critici per le prestazioni */
    ITCM0      (RWX) : ORIGIN = 0x00000000, LENGTH = 64k
    ITCM2      (RWX) : ORIGIN = 0x00010000, LENGTH = 64k

    /* PFLASH: Programma principale */
    PFLASH     (RX)  : ORIGIN = 0x00400000, LENGTH = 8M

    /* DFLASH: Dati non volatili */
    DFLASH     (RX)  : ORIGIN = 0x10000000, LENGTH = 128k

    /* DTCM: Stack, heap, e dati critici */
    DTCM0      (RW)  : ORIGIN = 0x20000000, LENGTH = 128k
    DTCM2      (RW)  : ORIGIN = 0x21800000, LENGTH = 128k

    /* SRAM: Dati generici */
    SRAM_STDBY (RW)  : ORIGIN = 0x20400000, LENGTH = 64k  /* SRAM Standby */
    SRAM0      (RW)  : ORIGIN = 0x20410000, LENGTH = 192k /* Restante SRAM0 */
    SRAM1      (RW)  : ORIGIN = 0x20440000, LENGTH = 256k
    SRAM2      (RW)  : ORIGIN = 0x20480000, LENGTH = 256k

    /* Flash UTEST */
    UTEST      (RX)  : ORIGIN = 0x1B000000, LENGTH = 8k 
}

/* Dimensione minima per heap e stack */
_Min_Heap_Size = 0x8;
_Min_Stack_Size = 0x400;    /* Required amount of stack.  Used by main(), then re-used as the interrupt stack after the kernel starts. */

/* Stack iniziale */
_estack = ORIGIN(DTCM2) + LENGTH(DTCM2);

/* Sezioni del linker */
SECTIONS
{
    /* Vettore ISR */
    .isr_vector :
    {
        __vector_table = .;
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > ITCM0

    /* Sezione codice e dati readonly */
    .text :
    {
        __coderom_start__ = .;
        *(.text*)        
        *(.rodata*)      
        *(.constdata*)   
        _etext = .;      
        KEEP(*(.init))   
    } > PFLASH

    /* Dati inizializzati (RAM) */
    .data :
    {
        . = ALIGN(8);
        __data_start__ = .;
        *(.data)         
        *(.data.*)       
        *(vtable)
        _edata = .;
    } > DTCM0 AT > PFLASH

    /* Dati non inizializzati (RAM) */
    .bss :
    {
        . = ALIGN(8);
        __bss_start__ = .;
        *(.bss)
        *(.bss.*)
        _ebss = .;
    } > SRAM0

    /* Standby ram */
    .standby_ram :
    {
        . = ALIGN(8);
        *(.standby_ram)
    } > SRAM_STDBY

    /* Dati di test */
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

    ASSERT(__stack_end__ <= ORIGIN(DTCM2) + LENGTH(DTCM2), "Stack overflow in DTCM2!")
}

/* Entry point */
ENTRY(Reset_Handler)

/* Stack iniziale */
PROVIDE(_stack = _estack);
