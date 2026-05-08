#ifndef MODEL_META_H
#define MODEL_META_H

#include "types.h"

typedef struct {
    layer_type_t type;
    tensor_shape_t input_shape;
    tensor_shape_t output_shape;
    uint32_t weight_offset;
    uint32_t bias_offset;
} layer_meta_t;

typedef struct {
    layer_meta_t *layers;
    uint32_t     num_layers;
    uint32_t     layers_capacity;
    tensor_shape_t model_input_shape;
    tensor_shape_t model_output_shape;
} model_meta_t;

typedef struct {
    uint32_t total_weights_size;
    uint32_t max_activations_size;
    uint32_t max_intermediate_size;
} memory_requirements_t;

extern const model_meta_t g_model_meta;
extern const memory_requirements_t g_memory_requirements;

#endif /* MODEL_META_H */
