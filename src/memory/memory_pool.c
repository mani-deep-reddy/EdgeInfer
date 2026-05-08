#include "memory_pool.h"
#include <string.h>

/* TODO: implement dynamic pool sizing from TARGET_RAM_SIZE - POOL_HEADROOM */
static uint8_t static_pool[64 * 1024]; /* temporary minimal pool */

edgeinfer_status_t memory_pool_init(memory_pool_t *pool) {
    /* TODO: size pool from model metadata via TARGET_RAM_SIZE - POOL_HEADROOM */
    pool->base = static_pool;
    pool->total_size = sizeof(static_pool);
    pool->used = 0;
    memset(pool->base, 0, pool->total_size);
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
