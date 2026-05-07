#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

static const char *level_strings[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
};

void logger_init(void) {
    /* Platform-specific initialization (e.g., UART setup) */
}

void logger_log(log_level_t level, const char *module, const char *fmt, ...) {
#if ENABLE_LOGGING
    va_list args;
    va_start(args, fmt);
    printf("[%s][%s] ", level_strings[level], module);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
#else
    (void)level;
    (void)module;
    (void)fmt;
#endif
}
