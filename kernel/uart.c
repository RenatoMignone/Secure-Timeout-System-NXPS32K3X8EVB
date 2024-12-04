#include "uart.h"

void UART_init(void) {
    UART0_CTRL = 0x00000001; // Enable UART, for example
    UART0_BAUDDIV = 9600;    // Set baud rate to 9600
}

void UART_printf(const char *s) {
    while (*s) {
        while (!(UART0_STATE & 0x01)); // Wait until UART is ready
        UART0_DATA = *s++;            // Send character
    }
}



