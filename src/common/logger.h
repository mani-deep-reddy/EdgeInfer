#ifndef LOGGER_H
#define LOGGER_H

void logger_init(void);
void logger_log(const char *module, const char *fmt, ...);

#define LOG_INFO(mod, fmt, ...)  logger_log(mod, fmt, ##__VA_ARGS__)
#define LOG_ERROR(mod, fmt, ...) logger_log(mod, fmt, ##__VA_ARGS__)

#endif /* LOGGER_H */
