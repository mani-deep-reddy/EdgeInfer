#include "memory_pool.h"
#include "logger.h"
#include <string.h>

static uint8_t static_pool[MAX_INPUT_BUFFER_SIZE + MAX_OUTPUT_BUFFER_SIZE + MAX_WORKSPACE_SIZE];

edgeinfer_status_t memory_pool_init(memory_pool_t *pool) {
    pool->base = static_pool;
    pool->total_size = sizeof(static_pool);
    pool->used = 0;
    memset(pool->base, 0, pool->total_size);
    LOG_INFO("memory", "Pool initialized: %u bytes", pool->total_size);
    return EDGEINFER_OK;
}

void *memory_pool_alloc(memory_pool_t *pool, uint32_t size) {
    if (pool->used + size > pool->total_size) {
        LOG_ERROR("memory", "Pool overflow: need %u, have %u", size, pool->total_size - pool->used);
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
