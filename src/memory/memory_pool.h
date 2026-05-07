#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stdint.h>
#include "runtime_config.h"

/* Memory pool handle */
typedef struct {
    uint8_t  *base;
    uint32_t  total_size;
    uint32_t  used;
} memory_pool_t;

/* Initialize the static memory pool */
edgeinfer_status_t memory_pool_init(memory_pool_t *pool);

/* Allocate from the pool (no free — deterministic reuse only) */
void *memory_pool_alloc(memory_pool_t *pool, uint32_t size);

/* Reset pool to initial state (between inference runs) */
void memory_pool_reset(memory_pool_t *pool);

/* Get remaining free space */
uint32_t memory_pool_free(const memory_pool_t *pool);

#endif /* MEMORY_POOL_H */
