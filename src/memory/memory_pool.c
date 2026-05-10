#include "memory_pool.h"
#include "model_meta.h"
#include "runtime_config.h"
#include "logger.h"
#include <string.h>

static uint8_t static_pool[MODEL_MAX_ACTIVATIONS_SIZE + MODEL_MAX_INTERMEDIATE_SIZE] __attribute__((aligned(4)));

edgeinfer_status_t memory_pool_init(memory_pool_t *pool) {
    pool->base = static_pool;
    pool->total_size = sizeof(static_pool);
    pool->used = 0;
    memset(pool->base, 0, pool->total_size);
    if (sizeof(static_pool) > TARGET_RAM_SIZE - POOL_HEADROOM) {
        LOG_WARN("mem", "Pool size (%u) exceeds available RAM (%u)",
                 (unsigned)sizeof(static_pool), (unsigned)(TARGET_RAM_SIZE - POOL_HEADROOM));
    }
    return EDGEINFER_OK;
}

void *memory_pool_alloc(memory_pool_t *pool, uint32_t size) {
    /* TODO: bump allocator with overflow check */
    if (pool->used + size > pool->total_size) {
        return NULL;
    }
    void *ptr = pool->base + pool->used;
    pool->used += size;
    return ptr;
}

void memory_pool_reset(memory_pool_t *pool) {
    pool->used = 0;
}

uint32_t memory_pool_free(const memory_pool_t *pool) {
    return pool->total_size - pool->used;
}
