#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdarg.h>

void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_printf(const char *fmt, ...);    /* formatted output (%s, %d, %u supported) */
void uart_vprintf(const char *fmt, va_list args);   /* va_list variant of uart_printf */
void uart_print_float(float val);          /* print float as decimal string */

#endif /* UART_H */
