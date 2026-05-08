#ifndef BUFFER_LAYOUT_H
#define BUFFER_LAYOUT_H

#include <stdint.h>
#include "types.h"

typedef enum {
    BUF_INPUT,
    BUF_OUTPUT,
    BUF_WORKSPACE,
    BUF_COUNT
} buffer_id_t;

typedef struct {
    uint8_t  *base;
    uint32_t  size;
    uint32_t  offset;
} buffer_region_t;

typedef struct {
    buffer_region_t regions[BUF_COUNT];
} buffer_layout_t;

void buffer_layout_init(buffer_layout_t *layout, uint8_t *pool_base, uint32_t pool_size);
uint8_t *buffer_layout_get(const buffer_layout_t *layout, buffer_id_t id);

#endif /* BUFFER_LAYOUT_H */
