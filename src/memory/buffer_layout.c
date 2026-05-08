#include "buffer_layout.h"

void buffer_layout_init(buffer_layout_t *layout, uint8_t *pool_base, uint32_t pool_size) {
    /* TODO: carve pool into input, workspace, output regions */
    int i;
    for (i = 0; i < BUF_COUNT; i++) {
        layout->regions[i].base = pool_base;
        layout->regions[i].size = pool_size / BUF_COUNT;
        layout->regions[i].offset = 0;
    }
    (void)pool_size;
}

uint8_t *buffer_layout_get(const buffer_layout_t *layout, buffer_id_t id) {
    /* TODO: return base pointer of the requested region */
    if (id >= BUF_COUNT) return NULL;
    return layout->regions[id].base;
}
