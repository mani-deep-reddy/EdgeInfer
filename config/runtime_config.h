#ifndef RUNTIME_CONFIG_H
#define RUNTIME_CONFIG_H

/* Hardware-specific constants */
#define TARGET_RAM_SIZE     (1024 * 1024)   /* 1 MB — matches platform/arm/linker.ld */
#define POOL_HEADROOM       (16 * 1024)     /* 16 KB — stack, .data, .bss overhead */

/* Feature flags */
#define ENABLE_LOGGING      1
#define ENABLE_PROFILING    0

#endif /* RUNTIME_CONFIG_H */
