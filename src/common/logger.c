#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

void logger_init(void) {
    /* TODO: platform-specific init (e.g., UART) */
}

void logger_log(const char *module, const char *fmt, ...) {
    /* TODO: implement with proper log levels */
    va_list args;
    va_start(args, fmt);
    printf("[%s] ", module);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}
