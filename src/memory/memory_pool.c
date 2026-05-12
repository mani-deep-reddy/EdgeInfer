#include "memory_pool.h"
#include "model_meta.h"
#include "runtime_config.h"
#include "logger.h"
#include <string.h>

static uint8_t static_pool[MODEL_INPUT_SIZE + MODEL_OUTPUT_SIZE + MODEL_MAX_ACTIVATIONS_SIZE + MODEL_MAX_INTERMEDIATE_SIZE] __attribute__((aligned(4)));   /* compile-time static pool backing */

edgeinfer_status_t memory_pool_init(memory_pool_t *pool) {   /* init pool: base, size, zero-fill */
    pool->base = static_pool;
    pool->total_size = sizeof(static_pool);
    pool->used = 0;
    memset(pool->base, 0, pool->total_size);                 /* zero pool on init */
    if (sizeof(static_pool) > TARGET_RAM_SIZE - POOL_HEADROOM) {
        LOG_WARN("mem", "Pool size (%u) exceeds available RAM (%u)",
                 (unsigned)sizeof(static_pool), (unsigned)(TARGET_RAM_SIZE - POOL_HEADROOM));
    }
    return EDGEINFER_OK;
}

void *memory_pool_alloc(memory_pool_t *pool, uint32_t size) {   /* bump allocator with overflow check */
    if (pool->used + size > pool->total_size) {
        return NULL;
    }
    void *ptr = pool->base + pool->used;    /* current bump position */
    pool->used += size;                     /* advance the bump */
    return ptr;
}

void memory_pool_reset(memory_pool_t *pool) {   /* reset used counter (no free) */
    pool->used = 0;
}

uint32_t memory_pool_free(const memory_pool_t *pool) {   /* bytes remaining in pool */
    return pool->total_size - pool->used;
}
