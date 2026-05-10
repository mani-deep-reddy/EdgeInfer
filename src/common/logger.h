#ifndef LOGGER_H
#define LOGGER_H

#include "runtime_config.h"

#define LOG_LEVEL_DEBUG 0   /* lowest severity */
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3   /* highest severity */

typedef int log_level_t;   /* one of LOG_LEVEL_* constants */

void logger_init(void);     /* init logging backend (UART, semihosting, etc.) */
void logger_log(log_level_t level, const char *module, const char *fmt, ...);   /* log with level prefix */

#if ENABLE_LOGGING

#define LOG_DEBUG(mod, fmt, ...) \
    do { if (LOG_LEVEL <= LOG_LEVEL_DEBUG) logger_log(LOG_LEVEL_DEBUG, mod, fmt, ##__VA_ARGS__); } while (0)  /* debug-level message */
#define LOG_INFO(mod, fmt, ...) \
    do { if (LOG_LEVEL <= LOG_LEVEL_INFO) logger_log(LOG_LEVEL_INFO, mod, fmt, ##__VA_ARGS__); } while (0)    /* info-level message */
#define LOG_WARN(mod, fmt, ...) \
    do { if (LOG_LEVEL <= LOG_LEVEL_WARN) logger_log(LOG_LEVEL_WARN, mod, fmt, ##__VA_ARGS__); } while (0)    /* warning-level message */
#define LOG_ERROR(mod, fmt, ...) \
    do { if (LOG_LEVEL <= LOG_LEVEL_ERROR) logger_log(LOG_LEVEL_ERROR, mod, fmt, ##__VA_ARGS__); } while (0)  /* error-level message */

#else /* !ENABLE_LOGGING */

#define LOG_DEBUG(mod, fmt, ...)
#define LOG_INFO(mod, fmt, ...)
#define LOG_WARN(mod, fmt, ...)
#define LOG_ERROR(mod, fmt, ...)

#endif /* ENABLE_LOGGING */

#endif /* LOGGER_H */
