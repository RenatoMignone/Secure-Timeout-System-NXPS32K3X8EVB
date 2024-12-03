#include "uart.h"

void UART_init(void)
{
    // Set baud rate (Example: setting for 9600 baud rate with a system clock of 24MHz)
    // For a baud rate of 9600, you need to calculate the baud divisor.
    // This depends on your system clock, so adjust this part according to your platform.

    // Example calculation for 9600 baud rate:
    // If system clock is 24 MHz:
    // Divisor = 24000000 / 9600 = 2500 (This is just an example, modify as per your clock).

    UART0_BAUDDIV = 2500;  // Set baud rate divisor (this needs adjustment as per your clock)

    // Enable UART, TX, and RX
    UART0_CTRL = 0x301;  // Enable UART, Transmitter and Receiver (Check exact bit values for your platform)
}

void UART_printf(const char *s)
{
    while (*s != '\0') {
        // Wait for the UART to be ready to transmit (TX FIFO empty)
        while (UART0_STATE & (1 << 5)) {  // TXFE (Transmit FIFO Empty) bit is 1 when TX is ready
            // Busy-wait until TX is empty and ready for new data
        }

        // Send the character to UART0_DATA register
        UART0_DATA = (unsigned int)(*s);
        s++;  // Move to the next character
    }
}

