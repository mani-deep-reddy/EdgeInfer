#include "uart.h"
#include <stdarg.h>

/*
 * Minimal UART backend for QEMU mps2-an385
 * UART0 base address: 0x40004000
 */

#define UART0_BASE  0x40004000
#define UART0_DR    (*(volatile uint32_t *)(UART0_BASE + 0x00))
#define UART0_CTRL  (*(volatile uint32_t *)(UART0_BASE + 0x08))

#define UART_TX_ENABLE  (1 << 0)   /* control register: transmitter enable */
#define UART_RX_ENABLE  (1 << 1)   /* control register: receiver enable */

void uart_init(void) {
    UART0_CTRL = UART_TX_ENABLE | UART_RX_ENABLE;   /* enable UART for transmit and receive */
}

void uart_putc(char c) {
    if (c == '\n') {
        UART0_DR = '\r';
    }
    UART0_DR = (uint32_t)c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

static void print_dec(unsigned long val) {   /* print unsigned decimal */
    char buf[12];                              /* max 10 digits + null for 32-bit */
    char *p = buf + sizeof(buf) - 1;
    *p = '\0';
    do {
        *--p = '0' + (val % 10);
        val /= 10;
    } while (val);
    uart_puts(p);
}

void uart_vprintf(const char *fmt, va_list args) {   /* format engine: %s, %d, %u, %% */
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's': {
                    const char *s = va_arg(args, const char *);
                    uart_puts(s);
                    break;
                }
                case 'd': {
                    long v = va_arg(args, int);
                    if (v < 0) { uart_putc('-'); v = -v; }
                    print_dec((unsigned long)v);
                    break;
                }
                case 'u': {
                    unsigned long v = va_arg(args, unsigned int);
                    print_dec(v);
                    break;
                }
                case '%':
                    uart_putc('%');
                    break;
                default:
                    uart_putc('%');
                    uart_putc(*fmt);
                    break;
            }
        } else {
            uart_putc(*fmt);
        }
        fmt++;
    }
}

void uart_printf(const char *fmt, ...) {   /* formatted output via uart_vprintf */
    va_list args;
    va_start(args, fmt);
    uart_vprintf(fmt, args);
    va_end(args);
}

void uart_print_float(float val) {   /* print float as "0.000000" (6 decimal places) */
    if (val < 0) { uart_putc('-'); val = -val; }
    unsigned long whole = (unsigned long)val;
    print_dec(whole);
    uart_putc('.');
    unsigned long frac = (unsigned long)((val - (float)whole) * 1000000.0f + 0.5f);
    char buf[7];                      /* 6 digits + null */
    for (int i = 5; i >= 0; i--) {
        buf[i] = '0' + (frac % 10);
        frac /= 10;
    }
    buf[6] = '\0';
    uart_puts(buf);
}
