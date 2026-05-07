#include "types.h"

/* Clamp value between min and max */
float clamp(float val, float min_val, float max_val) {
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

/* Compute total elements from shape */
uint32_t tensor_num_elements(const tensor_shape_t *shape) {
    uint32_t count = 1;
    for (uint8_t i = 0; i < shape->ndim; i++) {
        count *= shape->dims[i];
    }
    return count;
}
