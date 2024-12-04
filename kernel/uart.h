#ifndef __PRINTF__
#define __PRINTF__

#include "FreeRTOS.h"

#define UART0_ADDRESS                         ( 0x4006A000 )  // Adjust this address if needed
#define UART0_DATA                            ( *( (volatile uint32_t *) ( UART0_ADDRESS + 0UL ) ) )  // Data register
#define UART0_STATE                           ( *( (volatile uint32_t *) ( UART0_ADDRESS + 4UL ) ) )  // Status register
#define UART0_CTRL                            ( *( (volatile uint32_t *) ( UART0_ADDRESS + 8UL ) ) )  // Control register
#define UART0_BAUDDIV                         ( *( (volatile uint32_t *) ( UART0_ADDRESS + 16UL ) ) )  // Baud rate divisor

void UART_init(void);
void UART_printf(const char *s);

#endif
