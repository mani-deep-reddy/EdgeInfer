#ifndef RUNTIME_CONFIG_H
#define RUNTIME_CONFIG_H

/* Buffer sizes */
#define MAX_INPUT_BUFFER_SIZE   (1024 * 1024)   /* 1 MB */
#define MAX_OUTPUT_BUFFER_SIZE  (1024 * 1024)   /* 1 MB */
#define MAX_WORKSPACE_SIZE      (2 * 1024 * 1024) /* 2 MB */

/* Feature flags */
#define ENABLE_LOGGING          1
#define ENABLE_PROFILING        0

/* Global limits */
#define MAX_LAYERS              128
#define MAX_INPUT_DIMS          4
#define MAX_OUTPUT_DIMS         4

#endif /* RUNTIME_CONFIG_H */
