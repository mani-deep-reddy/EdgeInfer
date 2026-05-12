#include "logger.h"
#include "uart.h"
#include <stdarg.h>

static const char *level_prefix(log_level_t level) {   /* map level to short prefix string */
    switch (level) {
        case LOG_LEVEL_DEBUG: return "[D]";
        case LOG_LEVEL_INFO:  return "[I]";
        case LOG_LEVEL_WARN:  return "[W]";
        case LOG_LEVEL_ERROR: return "[E]";
        default:              return "[?]";
    }
}

void logger_init(void) {   /* init logging backend (UART) */
#if ENABLE_LOGGING
    uart_init();
#endif
}

void logger_log(log_level_t level, const char *module, const char *fmt, ...) {   /* output formatted log message via UART */
    va_list args;           /* variadic argument list */
    va_start(args, fmt);
    uart_puts(level_prefix(level));
    uart_puts("[");
    uart_puts(module);
    uart_puts("] ");
    uart_vprintf(fmt, args);
    va_end(args);
    uart_puts("\n");
}
