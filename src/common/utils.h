#ifndef UTILS_H
#define UTILS_H

#include "types.h"

float    clamp(float val, float min_val, float max_val);      /* constrain val to [min_val, max_val] */
uint32_t tensor_num_elements(const tensor_shape_t *shape);   /* product of all dims up to ndim */

#endif /* UTILS_H */
