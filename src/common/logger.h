#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
} log_level_t;

void logger_init(void);
void logger_log(log_level_t level, const char *module, const char *fmt, ...);

#define LOG_DEBUG(mod, fmt, ...) logger_log(LOG_LEVEL_DEBUG, mod, fmt, ##__VA_ARGS__)
#define LOG_INFO(mod, fmt, ...)  logger_log(LOG_LEVEL_INFO,  mod, fmt, ##__VA_ARGS__)
#define LOG_WARN(mod, fmt, ...)  logger_log(LOG_LEVEL_WARN,  mod, fmt, ##__VA_ARGS__)
#define LOG_ERROR(mod, fmt, ...) logger_log(LOG_LEVEL_ERROR, mod, fmt, ##__VA_ARGS__)

#endif /* LOGGER_H */
