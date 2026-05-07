#ifndef BUFFER_LAYOUT_H
#define BUFFER_LAYOUT_H

#include "types.h"
#include "runtime_config.h"

/* Buffer region identifiers */
typedef enum {
    BUF_INPUT,
    BUF_OUTPUT,
    BUF_WORKSPACE,
    BUF_COUNT
} buffer_id_t;

/* Memory region descriptor */
typedef struct {
    uint8_t  *base;
    uint32_t  size;
    uint32_t  offset;
} buffer_region_t;

/* Layout descriptor for the entire memory pool */
typedef struct {
    buffer_region_t regions[BUF_COUNT];
} buffer_layout_t;

/* Initialize buffer layout with pool base address */
void buffer_layout_init(buffer_layout_t *layout, uint8_t *pool_base, uint32_t pool_size);

/* Get pointer to a specific buffer region */
uint8_t *buffer_layout_get(const buffer_layout_t *layout, buffer_id_t id);

#endif /* BUFFER_LAYOUT_H */
