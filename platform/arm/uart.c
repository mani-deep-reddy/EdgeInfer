#include "uart.h"

/*
 * Minimal UART backend for QEMU mps2-an385
 * UART0 base address: 0x40004000
 */

#define UART0_BASE  0x40004000
#define UART0_DR    (*(volatile uint32_t *)(UART0_BASE + 0x00))

void uart_init(void) {
    /* UART is typically pre-configured in QEMU */
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
