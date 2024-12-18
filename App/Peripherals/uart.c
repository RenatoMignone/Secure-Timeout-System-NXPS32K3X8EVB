#include "uart.h"

void UART_init(void)
{
    // Configure the baud rate
    LPUART_BRR = 16;

    // Enable the transmitter and receiver
    LPUART_CR1 = (1 << 3) | (1 << 2); // TE (Transmitter enable) and RE (Receiver enable)

    // Enable the UART
    LPUART_CR1 |= (1 << 0); // UE (USART enable)

    UART_printf("\nUART initialized!\n\n");
}

void UART_printf(const char *s) 
{
    while (*s != '\0') {
        // Wait until the transmit data register is empty

        // Write the character to the TDR
        LPUART_TDR = (unsigned int)(*s);
        s++;
    }
}

void UART_putChar(char c) 
{
    // Wait until the transmit data register is empty
   
    // Write the character to the TDR
    LPUART_TDR = (unsigned int)c;
}
