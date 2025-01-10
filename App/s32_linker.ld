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
    SRAM0      (RW)  : ORIGIN = 0x20400000, LENGTH = 256k
    SRAM1      (RW)  : ORIGIN = 0x20440000, LENGTH = 256k
    SRAM2      (RW)  : ORIGIN = 0x20480000, LENGTH = 256k

    /* Flash UTEST  (Non implementata nel linker)
    UTEST      (RX)  : ORIGIN = 0x1B000000, LENGTH = 8k */
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
        *(.text*)        /* Codice */
        *(.rodata*)      /* Dati readonly */
        *(.constdata*)   /* Costanti */
        _etext = .;      /* Fine del codice */
        KEEP(*(.init))   /* Inizializzazione */
    } > PFLASH

    /* Dati inizializzati (RAM) */
    .data :
    {
        . = ALIGN(8);
        _data = .;
        _sdata = .;
        *(vtable)
        *(.data)
        _edata = .;
    } > DTCM0 AT > PFLASH

    /* Dati non inizializzati (RAM) */
    .bss :
    {
        . = ALIGN(8);
        _bss = .;
        _sbss = .;
        *(.bss)
        _ebss = .;
    } > SRAM0

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
    __StackTop = ORIGIN(DTCM2) + LENGTH(DTCM2);
    __StackLimit = __StackTop - _Min_Stack_Size;

    ASSERT(__StackLimit >= _heap_top, "Overflow: stack e heap si sovrappongono!")
}

/* Entry point */
ENTRY(Reset_Handler)

/* Stack iniziale */
PROVIDE(_stack = _estack);
