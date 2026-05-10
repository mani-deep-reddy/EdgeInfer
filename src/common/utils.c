#include "utils.h"

float clamp(float val, float min_val, float max_val) {   /* clamp val to [min_val, max_val] */
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

uint32_t tensor_num_elements(const tensor_shape_t *shape) {   /* product of all dims up to ndim */
    uint32_t count = 1;       /* running product */
    for (uint8_t i = 0; i < shape->ndim; i++) {
        count *= shape->dims[i];
    }
    return count;
}
