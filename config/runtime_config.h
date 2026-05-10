#ifndef RUNTIME_CONFIG_H
#define RUNTIME_CONFIG_H

/* Hardware-specific constants */
#define TARGET_RAM_SIZE     (1024 * 1024)   /* 1 MB — matches platform/arm/linker.ld */
#define POOL_HEADROOM       (16 * 1024)     /* 16 KB — stack, .data, .bss overhead */

/* Feature flags */
#define ENABLE_LOGGING      1
#define ENABLE_PROFILING    0

/* Log level (value from LOG_LEVEL_* constants in logger.h) */
#define LOG_LEVEL           LOG_LEVEL_INFO   /* messages below this are compiled out */

#endif /* RUNTIME_CONFIG_H */
