#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stdint.h>
#include "types.h"

typedef struct {
    uint8_t  *base;
    uint32_t  total_size;
    uint32_t  used;
} memory_pool_t;

edgeinfer_status_t memory_pool_init(memory_pool_t *pool);
void *memory_pool_alloc(memory_pool_t *pool, uint32_t size);
void memory_pool_reset(memory_pool_t *pool);
uint32_t memory_pool_free(const memory_pool_t *pool);

#endif /* MEMORY_POOL_H */
