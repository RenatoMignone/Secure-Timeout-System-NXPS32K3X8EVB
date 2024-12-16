// printf-stdarg.h
// Header for a minimal printf implementation based on stdarg

#ifndef PRINTF_STDARG_H
#define PRINTF_STDARG_H

#include <stdarg.h> 

int printf(const char *fmt, ...);

#ifndef putchar
#define putchar(c) UART_putChar(c)
#endif

#endif // PRINTF_STDARG_H