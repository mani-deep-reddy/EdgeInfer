#include "logger.h"
#include <stdio.h>
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

void logger_init(void) {   /* init logging backend */
#if ENABLE_LOGGING
    /* TODO: platform-specific init (e.g., UART) */
#endif
}

void logger_log(log_level_t level, const char *module, const char *fmt, ...) {   /* output formatted log message */
    va_list args;       /* variadic argument list */
    va_start(args, fmt);
#ifdef SEMIHOSTING
    /* ARM semihosting output via SVC 0x123456 */
    printf("%s[%s] ", level_prefix(level), module);
    vprintf(fmt, args);
    printf("\n");
#else
    printf("%s[%s] ", level_prefix(level), module);    /* host output via stdio */
    vprintf(fmt, args);
    printf("\n");
#endif
    va_end(args);
}
