#include "buffer_layout.h"
#include "model.h"
#include "utils.h"

void buffer_layout_init(buffer_layout_t *layout, uint8_t *pool_base, uint32_t pool_size) {
    uint32_t input_size   = tensor_num_elements(&g_model_meta.model_input_shape) * sizeof(float);
    uint32_t output_size  = tensor_num_elements(&g_model_meta.model_output_shape) * sizeof(float);

    layout->regions[BUF_INPUT].base   = pool_base;
    layout->regions[BUF_INPUT].size   = input_size;
    layout->regions[BUF_INPUT].offset = 0;

    layout->regions[BUF_OUTPUT].base   = pool_base + input_size;
    layout->regions[BUF_OUTPUT].size   = output_size;
    layout->regions[BUF_OUTPUT].offset = input_size;

    layout->regions[BUF_WORKSPACE].base   = pool_base + input_size + output_size;
    layout->regions[BUF_WORKSPACE].size   = pool_size - input_size - output_size;
    layout->regions[BUF_WORKSPACE].offset = input_size + output_size;
}

uint8_t *buffer_layout_get(const buffer_layout_t *layout, buffer_id_t id) {
    /* TODO: return base pointer of the requested region */
    if (id >= BUF_COUNT) return NULL;
    return layout->regions[id].base;
}
