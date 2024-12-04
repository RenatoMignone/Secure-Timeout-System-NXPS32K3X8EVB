#include "uart.h"

// Initialize UART
void UART_init(void)
{
    // Set baud rate (Example: setting for 9600 baud rate with a system clock of 24 MHz)
    // Formula: Divisor = System Clock / (16 * Baud Rate)
    // For 24 MHz clock and 9600 baud rate:
    // Divisor = 24,000,000 / (16 * 9600) = 156.25, so we round to 156

    UART0_BAUDDIV = 156;  // Set baud rate divisor (this is the correct divisor for 9600 baud)

    // Enable UART, Transmitter, and Receiver (check specific bit values for your platform)
    UART0_CTRL = 0x301;  // Enable UART, Transmitter, and Receiver for pl011 or similar
}

// UART printf function for sending characters over UART
void UART_printf(const char *s)
{
    while (*s != '\0') {
        // Wait for the UART to be ready to transmit (TX FIFO empty)
        while (UART0_STATE & (1 << 5)) {  // TXFE (Transmit FIFO Empty) bit
            // Busy-wait until TX FIFO is empty and ready for new data
        }

        // Send the character to UART0_DATA register
        UART0_DATA = (unsigned int)(*s);
        s++;  // Move to the next character
    }
}
